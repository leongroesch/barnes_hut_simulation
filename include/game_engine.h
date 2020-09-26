#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include <random> 
#include <memory>
#include <iostream>
#include <cmath>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "body.h"
#include "barnes_hut.h"
#include "json_parser.h"
#include "fps.h"

class game_engine{
  private:
    float field_size{0};

    sf::RenderWindow window;
    sf::View view;

    sf::Clock update_clock;
    std::vector<std::shared_ptr<body>> bodys;

    barnes_hut barnes;
    FPS fps;

    bool paused{true};

    void random_bodys();
    void event_handler();
    void update(sf::Time elapsed_time);
    void draw();
  public:
    game_engine(sf::VideoMode video_mode, std::string title);
    void main_loop();
};

#endif