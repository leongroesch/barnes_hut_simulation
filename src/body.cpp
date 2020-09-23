#include "body.h"

#include <iostream>


/* ##################### Constructor #####################*/

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

/* ##################### Private #####################*/

void body::compute_collision()
{
  static constexpr float loss_factor = 0.7;
  if(combine_with)
  {

    mass += loss_factor * combine_with->get_mass();

    float area = M_PI * std::pow(get_radius(), 2);
    area += M_PI * std::pow(combine_with->get_radius(), 2) * loss_factor;
    circle_shape.setRadius( std::sqrt(area / M_PI));

    velocity.x += combine_with->get_velocity().x;
    velocity.y += combine_with->get_velocity().y;

    combine_with.reset();
  }

}

/* ##################### Public #####################*/
void body::update(sf::Time elapsed_time)
{
  // Calculate movement
  acceleration.x = force.x / mass;
  acceleration.y = force.y / mass;

  force.x = 0; force.y = 0;
  velocity += acceleration * (elapsed_time.asSeconds() * (float)36e4);

  circle_shape.move(velocity * (elapsed_time.asSeconds() * (float)36e4));

  compute_collision();
}

void body::apply_force(sf::Vector2f other_center, double other_mass)
{ 
  double square_dist = get_square_distance(other_center);
  if (square_dist == 0)
    return;
  double force_abs = (big_g  * mass * other_mass) / square_dist;

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
    if (!remove  && !other->get_remove() &&
        mass > other->get_mass()){
      other->set_remove();
      combine_with = other;
    }
  }

}

/********** Setter **********/

void body::set_remove()
{
  remove = true;
}


/********** Getter **********/

bool body::get_remove() const
{
  return remove;
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

float body::get_radius() const
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

/********** Debug **********/
void body::display() const
{
  std::cout << mass << std::endl;

  std::cout << "Acceleration : (" << acceleration.x << "|" << acceleration.y << ")\n"; 

  std::cout << "Velocity : (" << velocity.x << "|" << velocity.y << ")\n";
  
  std::cout << "Position : (" << get_center().x << "|" << get_center().y << ")\n";
}