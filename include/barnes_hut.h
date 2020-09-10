#ifndef _BARNES_HUT_H_
#define _BARNES_HUT_H_

#include <memory>

#include <SFML/Graphics.hpp>

#include "body.h"


class barnes_hut 
{
  private:
    class node
    {
      public:
        sf::Vector2f center_of_mass;
        float mass{0};

        sf::Rect<uint> area; 
        std::shared_ptr<node> children[4];
        std::shared_ptr<body> contained_body;

        node(sf::Vector2u position, sf::Vector2u size) : area(position, size) {};

        bool is_internal()
        {
          return mass != 0 && !contained_body;
        }

        bool is_external()
        {
          return contained_body != nullptr;
        }

        void add_body(std::shared_ptr<body> new_body)
        {
          if (!new_body)
            return;
          float old_mass = mass;
          mass += new_body->get_mass();
          center_of_mass.x = (center_of_mass.x * old_mass + new_body->getPosition().x * new_body->get_mass()) / mass;
          center_of_mass.y = (center_of_mass.y * old_mass + new_body->getPosition().y * new_body->get_mass()) / mass;
        }

        void create_children()
        {
          uint left_width = area.width/2 + (area.width%2);
          uint right_width = area.width/2;
          uint top_height = area.height/2 + (area.height%2);
          uint bottom_height = area.height/2;

          for(uint i = 0; i < 4; i++)
          {
            sf::Vector2u child_position;
            child_position.x = area.left + left_width * (i%2);
            child_position.y = area.top + top_height * (i >= 2);
            sf::Vector2u child_area;
            child_area.x = (i%2) ? left_width : right_width;
            child_area.y = (i<2) ? top_height : bottom_height;
            children[i] = std::make_shared<node>(node(child_position ,child_area));
          }
        }
    };

    std::shared_ptr<node> root;

    std::vector<std::shared_ptr<node>> nodes;

    constexpr static float theta{1.0};
    
    void sub_tree_insert_body(std::shared_ptr<body> new_body, std::shared_ptr<node> sub_tree);
    void sub_tree_apply_forces(std::shared_ptr<node> sub_tree, std::shared_ptr<body> apply_body);

  public:
    barnes_hut(sf::Vector2u window_size);
    void reset();
    void insert_body(std::shared_ptr<body> new_body);
    void draw_rects(sf::RenderWindow& window);
    void apply_forces(std::shared_ptr<body> apply_body);
};

#endif 