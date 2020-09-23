#ifndef _BARNES_HUT_H_
#define _BARNES_HUT_H_

#include <memory>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "body.h"


class barnes_hut 
{
  private:
    class node
    {
      public:
        sf::Vector2f center_of_mass{0, 0};
        float mass{0};

        sf::Rect<float> area; 
        std::shared_ptr<node> children[4];
        std::shared_ptr<body> contained_body;

        node(sf::Vector2f position, sf::Vector2f size) : area(position, size){};

        bool is_internal() const;

        bool is_external() const;

        void add_body(std::shared_ptr<body> new_body);  

        void create_children();
    };

    std::shared_ptr<node> root;

    std::vector<std::shared_ptr<node>> nodes;

    constexpr static float theta{1.0};
    
    void sub_tree_insert_body(std::shared_ptr<body> new_body, std::shared_ptr<node> sub_tree);
    void sub_tree_apply_forces(std::shared_ptr<node> sub_tree, std::shared_ptr<body> apply_body);

  public:
    barnes_hut(float field_size);
    void reset();
    void insert_body(std::shared_ptr<body> new_body);
    void draw_rects(sf::RenderWindow& window) const;
    void apply_forces(std::shared_ptr<body> apply_body);
};

#endif 