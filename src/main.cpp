#include <iostream>
#include <SFML/Graphics.hpp>

#include "game_engine.h"

int main() {
    game_engine engine(sf::VideoMode(1200, 900), "Game Engine");
    engine.main_loop();
    return 0;
}