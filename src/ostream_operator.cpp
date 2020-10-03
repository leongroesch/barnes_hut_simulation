
#include "ostream_operator.h"

std::ostream& operator << (std::ostream& os, const sf::Vector2f& vec)
{
  os << "(" << vec.x << "|" << vec.y << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const body& b)
{
  os << "Position: (" << b.get_center().x << "|" << b.get_center().y << ")\n";
  return os;
}