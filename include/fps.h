#ifndef _FPS_H_
#define _FPS_H_

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

class FPS
{
  private:
    sf::Font font;
    sf::Text text;

    sf::Clock update_clock;

  public:
    FPS();
    void update(sf::Time elapsed_time);
    sf::Text get_text();
};

#endif 