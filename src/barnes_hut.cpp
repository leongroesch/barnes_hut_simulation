#include "barnes_hut.h"

barnes_hut::barnes_hut(sf::Vector2u window_size)  
        : root(std::make_shared<node>(node(sf::Vector2u(0,0), window_size )))
{
   nodes.push_back(root); 
}

/* ##################### Private #####################*/

void barnes_hut::sub_tree_insert_body(std::shared_ptr<body> new_body, std::shared_ptr<node> sub_tree)
{
  if (!sub_tree->contained_body && sub_tree->mass == 0){
    sub_tree->add_body(new_body);
    sub_tree->contained_body = new_body;
  }
  else if ( sub_tree->is_internal() ){
    sub_tree->add_body(new_body);
    for(uint i = 0; i < 4; i++)
      if(sub_tree->children[i]->area.contains(sf::Vector2u(new_body->getPosition())))
        sub_tree_insert_body(new_body, sub_tree->children[i]);
  }
  else{
    sub_tree->add_body(new_body);
    auto reinsert_body = sub_tree->contained_body;
    sub_tree->contained_body.reset();
    // Create Children
    sub_tree->create_children();
    
    for(uint i = 0; i < 4; i++)
      nodes.push_back(sub_tree->children[i]);
    //Insert bodys in new children
    for(uint8_t i = 0; i < 4; i++)
      if(sub_tree->children[i]->area.contains(sf::Vector2u(new_body->getPosition())))
        sub_tree_insert_body(new_body, sub_tree->children[i]);
    for(uint8_t i = 0; i < 4; i++)
      if(sub_tree->children[i]->area.contains(sf::Vector2u(reinsert_body->getPosition())))
        sub_tree_insert_body(reinsert_body, sub_tree->children[i]);
  }
}

void barnes_hut::sub_tree_apply_forces(std::shared_ptr<node> sub_tree, std::shared_ptr<body> apply_body)
{
  if(sub_tree->is_external() && sub_tree->contained_body != apply_body)
  {
    apply_body->apply_force(sub_tree->center_of_mass, sub_tree->mass);
    apply_body->check_collision(sub_tree->contained_body);
  }
  else if ( (sub_tree->area.width/apply_body->get_distance(sub_tree->center_of_mass)) < theta )
  {
    apply_body->apply_force(sub_tree->center_of_mass, sub_tree->mass);
  }
  else
  {
    for(uint i = 0; i < 4; i++)
      if(sub_tree->children[i] && sub_tree->children[i]->mass != 0)  
        sub_tree_apply_forces(sub_tree->children[i], apply_body);
  }

}

/* ##################### Public #####################*/
void barnes_hut::reset()
{
  root->mass = 0;
  for(uint i = 0; i < 4; i++)
  {
    root->children[i].reset();
    root->contained_body.reset();
  }
  nodes.clear();
  nodes.push_back(root);
}

void barnes_hut::insert_body(std::shared_ptr<body> new_body)
{
  if(new_body)
    sub_tree_insert_body(new_body, root);
}

void barnes_hut::draw_rects(sf::RenderWindow& window)
{
  sf::RectangleShape rect;
  rect.setFillColor(sf::Color::Transparent);
  rect.setOutlineThickness(0.5);
  rect.setOutlineColor(sf::Color::White);
  for(auto n : nodes){
    if(n){
      rect.setSize(sf::Vector2f(n->area.width, n->area.height));
      rect.setPosition(n->area.left, n->area.top);
      window.draw(rect);
    }
  }
}

void barnes_hut::apply_forces(std::shared_ptr<body> apply_body)
{
  if(root)
    sub_tree_apply_forces(root, apply_body);
}