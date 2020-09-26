#include <fstream>

#include "json_parser.h"

body Json_parser::create_body_absolute(const Json::Value& properies)
{
  sf::Vector2f position(0, 0);
  sf::Vector2f velocity(0, 0);
  double mass = 0 ;
  float radius = 0;
  sf::Color color(0, 0, 0);

  if(properies["position"]["x"].isDouble())
    position.x = properies["position"]["x"].asFloat();
  if(properies["position"]["y"].isDouble())
    position.y = properies["position"]["y"].asFloat();
  
  if(properies["velocity"]["x"].isDouble())
    velocity.x = properies["velocity"]["x"].asFloat();
  if(properies["velocity"]["y"].isDouble())
    velocity.y = properies["velocity"]["y"].asFloat();
  
  if(properies["mass"].isDouble())
    mass = properies["mass"].asDouble();

  if(properies["radius"].isDouble())
    radius = properies["radius"].asFloat();
  
  if(properies["color"]["r"].isInt())
    color.r = properies["color"]["r"].asInt();
  if(properies["color"]["g"].isInt())
    color.g = properies["color"]["g"].asInt();
  if(properies["color"]["b"].isInt())
    color.b = properies["color"]["b"].asInt();
  
  return body(position, velocity, mass, radius, color);
}

body Json_parser::create_body_centralized(const Json::Value& properies)
{
  sf::Vector2f position(0, 0);
  sf::Vector2f velocity(0, 0);
  double mass = 0 ;
  float radius = 0;
  sf::Color color(0, 0, 0);

  if(properies["distance"].isDouble())
    position.x = properies["distance"].asFloat();
  
  float orbit_time = 0;
  if(properies["orbit_time_days"].isDouble())
    orbit_time = properies["orbit_time_days"].asDouble();
  
  if(orbit_time != 0)
  {
    velocity.y = (2* M_PI * position.x) / (orbit_time*secs_per_day);
  }

  if(properies["mass"].isDouble())
    mass = properies["mass"].asDouble();

  if(properies["radius"].isDouble())
    radius = properies["radius"].asFloat();
  
  if(properies["color"]["r"].isInt())
    color.r = properies["color"]["r"].asInt();
  if(properies["color"]["g"].isInt())
    color.g = properies["color"]["g"].asInt();
  if(properies["color"]["b"].isInt())
    color.b = properies["color"]["b"].asInt();
  
  return body(position, velocity, mass, radius, color);
}

std::vector<body> Json_parser::parse(std::string file_name, bool centralized)
{
  std::vector<body> ret;

  std::ifstream file(file_name, std::ifstream::binary);
  Json::Reader reader;
  Json::Value input;
  reader.parse(file, input);

  for(auto& b: input)
  {
    if(!centralized)
      ret.push_back(create_body_absolute(b));
    else
      ret.push_back(create_body_centralized(b));
  }

  return ret;
}