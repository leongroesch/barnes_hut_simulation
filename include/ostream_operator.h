#ifndef _OSTREAM_OPERATOR_H_
#define _OSTREAM_OPERATOR_H_

#include <iostream>
#include <SFML/Graphics.hpp>

#include "body.h" 

std::ostream& operator << (std::ostream& os, const sf::Vector2f& vec);
std::ostream& operator << (std::ostream& os, const body& b);


#endif