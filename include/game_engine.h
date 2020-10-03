#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include <random> 
#include <memory>
#include <iostream>
#include <cmath>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "body.h"
#include "barnes_hut.h"
#include "json_parser.h"
#include "fps.h"
#include "user_interface.h"

#include "ostream_operator.h"

class game_engine{
  private:
    float field_size{1e8};

    sf::RenderWindow window;
    sf::View view;

    tgui::Gui gui{window};
    tgui::Slider::Ptr slider;

    sf::Clock update_clock;
    std::vector<std::shared_ptr<body>> bodys;

    barnes_hut barnes;
    FPS fps;
    user_interface ui;

    bool paused{true};

    void event_handler();
    void update(sf::Time elapsed_time);
    void draw();
    void create_gui();

  public:
    game_engine(sf::VideoMode video_mode, std::string title, std::vector<std::string> arguments);
    void restart_simulation(float body_count, float min_mass, float max_mass);
    void main_loop();
};

#endif