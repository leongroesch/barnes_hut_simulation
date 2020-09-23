#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include <random> 
#include <memory>
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "body.h"
#include "barnes_hut.h"

class game_engine{
  private:
    constexpr static float field_size{30e10};

    sf::RenderWindow window;
    sf::View view;

    sf::Clock update_clock;
    sf::Clock fps_clock;
    std::vector<std::shared_ptr<body>> bodys;

    barnes_hut barnes;

    bool paused{true};
    uint16_t fps_count{0};

    void event_handler();
    void update();
    void draw();
  public:
    game_engine(sf::VideoMode video_mode, std::string title);
    void main_loop();
};

#endif