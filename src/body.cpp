#include "body.h"

#include <iostream>

body::body(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f acceleration, double mass, float radius) 
                      : circle_shape(radius), velocity(velocity), acceleration(acceleration), mass(mass)
{
   circle_shape.setPosition(position.x - radius, position.y - radius);
}

body::body(sf::Vector2f position, double mass, float radius, sf::Color color)
                      : circle_shape(radius), mass(mass)
{
  circle_shape.setPosition(position.x - radius, position.y - radius);
  circle_shape.setFillColor(color);
} 


void body::update(sf::Time elapsed_time)
{
  acceleration.x = force.x/mass;
  acceleration.y = force.y/mass;

  force.x = 0; force.y = 0;
  velocity += acceleration * elapsed_time.asSeconds();

  circle_shape.move(velocity * elapsed_time.asSeconds()); 
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
  float square_rad_sum = std::pow(get_radius()+other->get_radius(), 2);

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

sf::CircleShape body::get_circle_shape() const
{
  return circle_shape;
}

float body::get_distance(sf::Vector2f point) const
{
  sf::Vector2f tmp = get_center() - point;
  return std::sqrt( std::pow(tmp.x, 2) + std::pow(tmp.y, 2));
}

float body::get_square_distance(sf::Vector2f point) const
{
  sf::Vector2f tmp = get_center() - point;
  return std::pow(tmp.x, 2) + std::pow(tmp.y, 2);
}

double body::get_mass() const
{
  return mass;
}

int body::get_radius() const
{
  return circle_shape.getRadius();
}

sf::Vector2f body::get_center() const
{
  sf::Vector2f result = circle_shape.getPosition();
  result.x += circle_shape.getRadius();
  result.y += circle_shape.getRadius();
  return result;
}

sf::Vector2f body::get_velocity() const
{
  return velocity;
}