#include "user_interface.h"

user_interface::user_interface(sf::VideoMode video_mode, std::function<void(float, float, float, float)> start_callback)
                            : window(video_mode, "User interface"), gui(window)
{

  this->start_callback = start_callback;
  grid_layout = tgui::Grid::create();
  


  /* ###### Field size ###### */
  auto field_size_lb = tgui::Label::create("Field size: ");
  grid_layout->addWidget(field_size_lb, 0, 0, tgui::Padding(10));

  field_size_tb = tgui::TextBox::create();
  field_size_tb->setSize(textbox_width, field_size_lb->getSize().y);
  field_size_tb->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  field_size_tb->setText("1e8");
  grid_layout->addWidget(field_size_tb, 0, 1, tgui::Padding(10));

  /* ###### Number of bodies ###### */
  auto num_body_lb = tgui::Label::create("Number of bodys: ");
  grid_layout->addWidget(num_body_lb, 1, 0, tgui::Padding(10));

  num_body_tb = tgui::TextBox::create();
  num_body_tb->setSize(textbox_width, num_body_lb->getSize().y);
  num_body_tb->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  num_body_tb->setText("500");
  grid_layout->addWidget(num_body_tb, 1, 1, tgui::Padding(10));

  /* ###### Min mass ###### */
  auto min_mass_lb = tgui::Label::create("Minimum mass: ");
  grid_layout->addWidget(min_mass_lb, 2, 0, tgui::Padding(10));

  min_mass_tb = tgui::TextBox::create();
  min_mass_tb->setSize(textbox_width, num_body_lb->getSize().y);
  min_mass_tb->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  min_mass_tb->setText("1e8");
  grid_layout->addWidget(min_mass_tb, 2, 1, tgui::Padding(10));

  /* ###### Max mass ###### */
  auto max_mass_lb = tgui::Label::create("Maximum mass: ");
  grid_layout->addWidget(max_mass_lb, 3, 0, tgui::Padding(10));

  max_mass_tb = tgui::TextBox::create();
  max_mass_tb->setSize(textbox_width, num_body_lb->getSize().y);
  max_mass_tb->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  max_mass_tb->setText("1e14");
  grid_layout->addWidget(max_mass_tb, 3, 1, tgui::Padding(10));

    /* ###### Start Button ###### */
  start_button = tgui::Button::create();
  start_button->setText("Start");
  start_button->setSize(100, 50);
  start_button->connect("pressed", [&](){start_pressed();});
  grid_layout->addWidget(start_button, 4, 1, tgui::Padding(10) );

 
  gui.add(grid_layout);

  //grid_layout->addWidget(start_button, 10, 1);
}

void user_interface::start_pressed() const
{
  float field_size = std::atof( field_size_tb->getText().toAnsiString().c_str() ); 
  float num_bodys = std::atof( num_body_tb->getText().toAnsiString().c_str() );
  float min_mass = std::atof( min_mass_tb->getText().toAnsiString().c_str() );
  float max_mass = std::atof( max_mass_tb->getText().toAnsiString().c_str() );
  start_callback(field_size, num_bodys, min_mass, max_mass);
}

void user_interface::handle_event()
{
  sf::Event event;
  if(window.isOpen())
  {
    while(window.pollEvent(event))
      {
        if(event.type == sf::Event::Closed)
          window.close();
        else if (event.type == sf::Event::KeyPressed){
          if (event.key.code == sf::Keyboard::Escape)
            window.close();
          else if(event.key.code == sf::Keyboard::Return)
            start_pressed();
        }
        
        gui.handleEvent(event);
      }
  }
}

void user_interface::draw()
{
  window.clear(sf::Color::White);
  gui.draw();
  window.display();
}

// s