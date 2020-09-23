#ifndef _BODY_H_
#define _BODY_H_

#include <iostream>

#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
class body{
  private:
    constexpr static double big_g{6.67e-11};

    sf::CircleShape circle_shape;

    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Vector2f force;

    double mass;

    bool remove{false};
    std::shared_ptr<body> combine_with;

    void compute_collision();


  public:
    body(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f acceleration, double mass, float radius);
    body(sf::Vector2f position, double mass, float radius, sf::Color color);

    void update(sf::Time elapsed_time);
    void apply_force(sf::Vector2f other_center, double other_mass);
    void check_collision(std::shared_ptr<body> other);

    void set_remove();

    bool get_remove() const;
    sf::CircleShape get_circle_shape() const;
    float get_distance(sf::Vector2f point) const;
    float get_square_distance(sf::Vector2f point) const;
    double get_mass() const;
    float get_radius() const;
    sf::Vector2f get_center() const;
    sf::Vector2f get_velocity() const;

    void display() const;


};
#endif