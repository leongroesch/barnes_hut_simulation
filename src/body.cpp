#include "body.h"

#include <iostream>

body::body(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f acceleration, double mass, float radius) 
                      : sf::CircleShape(radius), velocity(velocity), acceleration(acceleration), mass(mass)
{
   setPosition(position.x - getRadius(), position.y - getRadius());
}

body::body(sf::Vector2f position, double mass, float radius, sf::Color color)
                      : sf::CircleShape(radius), mass(mass)
{
  setPosition(position.x - getRadius(), position.y - getRadius());
  setFillColor(color);
} 


void body::update(sf::Time elapsed_time)
{
  acceleration.x = force.x/mass;
  acceleration.y = force.y/mass;

  force.x = 0; force.y = 0;
  velocity += acceleration * elapsed_time.asSeconds();

  move(velocity * elapsed_time.asSeconds()); 
}

void body::apply_force(sf::Vector2f other_center, double other_mass)
{ 
  double square_dist = get_square_distance(other_center);
  if (square_dist == 0)
    return;
  double force_abs = (big_g * mass * other_mass) / square_dist;

  float force_x = (other_center.x - get_center().x) / get_distance(other_center);
  float force_y = (other_center.y - get_center().y) / get_distance(other_center);

  this->force.x += force_abs * force_x;
  this->force.y += force_abs * force_y;
}

void body::check_collision(std::shared_ptr<body> other)
{
  float square_dist = other->get_square_distance(get_center());
  float square_rad_sum = std::pow(getRadius()+other->getRadius(), 2);

  //Touch
  if (square_rad_sum == square_dist){
    return;
  }
  //Intersect
  else if (square_rad_sum > square_dist){
    sf::Vector2f tmp(velocity.x, velocity.y);
    tmp.x = tmp.x * -0.5; tmp.y = tmp.y * -0.5;
    set_velocity(tmp);

    tmp = other->get_velocity();
    tmp.x *= -0.5; tmp.y *= -0.5;
    other->set_velocity(tmp);
  }

}

void body::set_acceleration(sf::Vector2f acceleration)
{
    this->acceleration = acceleration;
}

void body::set_velocity(sf::Vector2f velocity)
{
  this->velocity = velocity;
}

float body::get_distance(sf::Vector2f point)
{
  sf::Vector2f tmp = get_center() - point;
  return std::sqrt( std::pow(tmp.x, 2) + std::pow(tmp.y, 2));
}

float body::get_square_distance(sf::Vector2f point)
{
  sf::Vector2f tmp = get_center() - point;
  return std::pow(tmp.x, 2) + std::pow(tmp.y, 2);
}

double body::get_mass()
{
  return mass;
}

sf::Vector2f body::get_center()
{
  sf::Vector2f result = getPosition();
  result.x += getRadius();
  result.y += getRadius();
  return result;
}

sf::Vector2f body::get_velocity()
{
  return velocity;
}