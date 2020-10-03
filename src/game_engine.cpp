
#include "game_engine.h"

/* ##################### Constructor #####################*/

using namespace std::placeholders;

game_engine::game_engine(sf::VideoMode video_mode, std::string title, std::vector<std::string> arguments) 
                : window(video_mode, title), barnes(field_size), ui(sf::VideoMode(400, 700), 
                  std::bind(&game_engine::restart_simulation, this, _1 , _2, _3 ))
{ 
  create_gui();

  bool import_json = false;
  bool centralized = false;

  if(arguments.size() >= 2)
    import_json = true;
  if(import_json && arguments.at(0) == "centralized")
    centralized = true;

  if(import_json)
  {
    std::string file = "";
    file = "../initial_conditions/" + arguments.at(1) + ".json";

    for(auto& bod : Json_parser::parse(file, centralized))
    {
      auto max_dist = std::max(bod.get_center().x, bod.get_center().y);
      field_size = std::max(field_size, max_dist );
      bodys.push_back(std::make_shared<body>(bod));
      //barnes.insert_body(bodys.back());
    }
    field_size = std::ceil(field_size + field_size/100);
  }

  sf::FloatRect view_rect(-field_size, -field_size, field_size*2, field_size*2);
  view.reset(view_rect);
  barnes.reset(view_rect);

  field_size = 2*field_size;
  window.setView(view);
}

/* ##################### Private #####################*/

void game_engine::event_handler()
{
  sf::Event event;
  float scrool_speed = field_size/100;
  while (window.pollEvent(event)){
      if (event.type == sf::Event::Closed){
        window.close();
      }
      else if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Space){
          paused = !paused;
          std::cout << "Toggle Pause\n";
        }
        else if (event.key.code == sf::Keyboard::Up)
          view.move(0,  scrool_speed * -1);
        else if (event.key.code == sf::Keyboard::Down)
          view.move(0, scrool_speed);
        else if (event.key.code == sf::Keyboard::Left)
          view.move(scrool_speed *-1, 0);
        else if (event.key.code == sf::Keyboard::Right)
          view.move(scrool_speed, 0);
        else if (event.key.code == sf::Keyboard::Escape)
          window.close();
      }
      else if (event.type == sf::Event::MouseWheelScrolled)
      {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
          if (event.mouseWheelScroll.delta < 0)
            view.zoom(0.9);
          else
            view.zoom(1.1);
        }
      }
      window.setView(view);
      gui.handleEvent(event);
  }
  ui.handle_event();
}

void game_engine::update(sf::Time elapsed_time)
{
  if (!paused)
  {
    fps.update(elapsed_time);
    barnes.reset();
    for(auto& x : bodys)
    {
      barnes.insert_body(x);
    }

    /* Barnes Hut */

    for(auto& x : bodys)
    {
      barnes.apply_forces(x);
    }

    for(auto it = bodys.begin(); it != bodys.end(); )
    {
      (*it)->update(elapsed_time, slider->getValue());

      if( std::isnan((*it)->get_velocity().x) || std::isnan((*it)->get_velocity().y) )
      {
        paused = true;
      }
      if((*it)->get_remove())
      {
        it = bodys.erase(it);
      }
      else
        it++;
      
    }
  }
}

void game_engine::draw()
{
  window.clear();
  float zoom_factor = window.getSize().x / view.getSize().x;
  for(auto x : bodys)
  {
    // Only draw seeable objects
    if( x->get_radius()*zoom_factor >= 0.7 )
      window.draw(x->get_circle_shape());
  }

  window.setView(window.getDefaultView());
  //Draw stuff unaffected by view
  ui.draw();
  gui.draw();
  window.draw(fps.get_text());

  window.setView(view);

  // barnes.draw_rects(window);
  window.display();
}

void game_engine::create_gui()
{
  tgui::Label::Ptr label = tgui::Label::create("Simulation speed: ");
  label->setPosition(15, window.getSize().y-50);
  label->getRenderer()->setTextColor(tgui::Color::White);
  gui.add(label);

  slider = tgui::Slider::create(1, 301);
  slider->setSize(150, 10);
  slider->setPosition(20, window.getSize().y-20);
  gui.add(slider);
}

/* ##################### Public #####################*/

void game_engine::restart_simulation(float body_count, float min_mass, float max_mass)
{
  bodys.clear();
  auto radius_factor = field_size/(5e2*max_mass);

  std::random_device random_device;
  std::mt19937 random_engine(random_device());
  std::uniform_real_distribution<float> dist_position(-(field_size/2), field_size/2);
  std::uniform_real_distribution<float> dist_velocity(field_size/1000 *-1, field_size/1000);
  std::uniform_real_distribution<float> dist_mass(min_mass, max_mass);

  std::cout << "Field_size: " << field_size << "\n";
  for(uint16_t i = 0; i < body_count; i++)
  {
    sf::Vector2f position(dist_position(random_engine), dist_position(random_engine));
    std::cout << "Position: " << position << "\n";
    sf::Vector2f velocity(0, 0); //(dist_velocity(random_engine), dist_velocity(random_engine));
    sf::Vector2f acceleration(0,0);
    double mass = dist_mass(random_engine);
    bodys.push_back(std::make_shared<body>(body(position, velocity, acceleration, mass, mass * radius_factor)));
    //barnes.insert_body(bodys.back());
  }

  //paused = false;

}

void game_engine::main_loop()
{
  while(window.isOpen()){
    auto elapsed_time = update_clock.restart();
    event_handler();
    update(elapsed_time);
    draw();
  }
}