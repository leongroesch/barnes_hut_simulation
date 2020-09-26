#include "fps.h"

FPS::FPS()
{
  if(!font.loadFromFile("../fonts/arial.ttf"))
    std::cerr << "Could not open font file:../fonts/arial.ttf ";
  text.setFont(font);
  text.setFillColor(sf::Color::White);
  text.setCharacterSize(20);
}

void FPS::update(sf::Time elapsed_time)
{
  if(update_clock.getElapsedTime().asSeconds()  > 0.5)
  {
    auto f_fps = std::floor(1/elapsed_time.asSeconds());
    std::stringstream stream;
    stream << std::fixed << std::setprecision(0) << f_fps;
    text.setString(stream.str());
    update_clock.restart();
  }
}

sf::Text FPS::get_text()
{
  return text;
}