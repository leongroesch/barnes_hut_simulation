#include <iostream>
#include <SFML/Graphics.hpp>

#include "game_engine.h"

std::vector<std::string> arg_to_string(int argc, char* argv[])
{
    std::vector<std::string> ret;
    for(int i = 1; i < argc; i++)
    {
        ret.push_back(argv[i]);
    }
    return ret;
}

int main(int argc, char* argv[]) 
{
    game_engine engine(sf::VideoMode(900, 900), "Game Engine", arg_to_string(argc, argv));
    engine.main_loop();
    return 0;
}