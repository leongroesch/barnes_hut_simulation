
#include "game_engine.h"



game_engine::game_engine(sf::VideoMode video_mode, std::string title) 
                : window(video_mode, title), barnes(field_size)
{
  view.reset(sf::FloatRect(0, 0, field_size, field_size));
  window.setView(view);

  
  const int num_body = 100;
  const double mass_enhancement = 10e15;

  std::random_device random_device;
  std::mt19937 random_engine(random_device());
  std::uniform_int_distribution<int> dist_position(0, field_size);
  std::uniform_int_distribution<int> dist_velocity(-500, 500);
  std::uniform_int_distribution<int> dist_mass_hole(100, 1000);
  std::uniform_int_distribution<int> dist_mass_small(100, 400);

  for(uint16_t i = 0; i < num_body; i++)
  {
    sf::Vector2f position(dist_position(random_engine), dist_position(random_engine));
    sf::Vector2f velocity(dist_velocity(random_engine), dist_velocity(random_engine));
    sf::Vector2f acceleration(0,0);
    double mass = (i%11 == 0) ? dist_mass_hole(random_engine) : dist_mass_small(random_engine);
    mass *= mass_enhancement;
    std::cout << "Mass: "<< mass << "\n";
    bodys.push_back(std::make_shared<body>(body(position, velocity, acceleration, mass, mass/mass_enhancement)));
    barnes.insert_body(bodys.back());
  }
  

 /* sf::Vector2f position(window.getSize().x/2, window.getSize().y/2);
  sf::Vector2f velocity(0, 0);//(dist_velocity(random_engine), dist_velocity(random_engine));
  sf::Vector2f acceleration(0,0);
  double mass = 1.988e16;
  // bodys.push_back(std::make_shared<body>(body(position, mass, 15, sf::Color::Red)));
  // barnes.insert_body(bodys.back());

  position.x = window.getSize().x/2 + 100; position.y = window.getSize().y/2 + 100;
  velocity.x = -10; velocity.y = -5;
  mass = 5.972e15;
  bodys.push_back(std::make_shared<body>(body(position, velocity, acceleration, mass, 10)));
  barnes.insert_body(bodys.back());

  // position.x = window.getSize().x/2 + 300; position.y = window.getSize().y/2 - 300;
  // velocity.x = -3; velocity.y = 20;
  // mass = 5.972e15;
  // bodys.push_back(std::make_shared<body>(body(position, velocity, acceleration, mass, 10)));
  // barnes.insert_body(bodys.back());

  position.x = window.getSize().x/2 - 100; position.y = window.getSize().y/2 - 100;
  velocity.x = 30; velocity.y = 40;
  mass = 5.972e15;
  bodys.push_back(std::make_shared<body>(body(position, velocity, acceleration, mass, 10)));
  barnes.insert_body(bodys.back());

  // position.x = window.getSize().x/2 - 300; position.y = window.getSize().y/2 + 300;
  // velocity.x = 10; velocity.y = -3;
  // mass = 5.972e15;
  // bodys.push_back(std::make_shared<body>(body(position, velocity, acceleration, mass, 10)));
  // barnes.insert_body(bodys.back());
  */
}
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
  }
}

void game_engine::update()
{
  auto elapsed_time = update_clock.getElapsedTime();
  if (!paused)
  {
    barnes.reset();
    for(auto x : bodys)
    {
      barnes.insert_body(x);
    }
    for(auto it = bodys.begin(); it != bodys.end(); )
    {
      (*it)->update(elapsed_time);
      barnes.apply_forces(*it);

      if((*it)->get_remove())
        it = bodys.erase(it);
      else
        it++;
      
    }
  }
  update_clock.restart();
}

void game_engine::draw()
{
  window.clear();
  for(auto x : bodys)
  {
    window.draw(x->get_circle_shape());
  }
  //barnes.draw_rects(window);
  window.display();
}

void game_engine::main_loop()
{
  while(window.isOpen()){
    event_handler();
    update();
    draw();
    if(fps_clock.getElapsedTime().asMilliseconds() > 1000)
    {
      fps_clock.restart();
      std::cout << fps_count << "\n";
      fps_count = 0; 
    }
    fps_count++; 
  }
}