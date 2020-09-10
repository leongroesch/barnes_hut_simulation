#ifndef _BODY_H_
#define _BODY_H_

#include <iostream>

#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
class body : public sf::CircleShape{
  private:
    constexpr static double big_g{6.67e-11};

    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Vector2f force;

    double mass;
  public:
    body(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f acceleration, double mass, float radius);
    body(sf::Vector2f position, double mass, float radius, sf::Color color);
    void update(sf::Time elapsed_time);
    void apply_force(sf::Vector2f other_center, double other_mass);
    void check_collision(std::shared_ptr<body> other);

    void set_acceleration(sf::Vector2f acceleration);
    void set_velocity(sf::Vector2f velocity);
    float get_distance(sf::Vector2f point);
    float get_square_distance(sf::Vector2f point);
    double get_mass();
    sf::Vector2f get_center();
    sf::Vector2f get_velocity();


};
#endif