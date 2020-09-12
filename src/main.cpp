#include <iostream>
#include <SFML/Graphics.hpp>

#include "game_engine.h"

int main() {
    game_engine engine(sf::VideoMode(900, 900), "Game Engine");
    engine.main_loop();
    return 0;
}