#include "barnes_hut.h"

barnes_hut::barnes_hut(float field_size)  
        : root(std::make_shared<node>(node(sf::Vector2f(0,0), sf::Vector2f(field_size, field_size) )))
{
   nodes.push_back(root); 
}
/* ##################### Node ########################*/


bool barnes_hut::node::is_internal() const
{
  return mass != 0 && !contained_body;
}

bool barnes_hut::node::is_external() const
{
  return contained_body != nullptr;
}

void barnes_hut::node::add_body(std::shared_ptr<body> new_body)
{
  if (!new_body)
    return;
  float old_mass = mass;
  mass += new_body->get_mass();
  
  center_of_mass.x = ((double)center_of_mass.x * old_mass + new_body->get_center().x * new_body->get_mass())/mass;
  center_of_mass.y = ((double)center_of_mass.y * old_mass + new_body->get_center().y * new_body->get_mass())/mass;
}

void barnes_hut::node::create_children()
{
  sf::Vector2f child_area(floor(area.width/2), floor(area.height/2));

  for(uint i = 0; i < 4; i++)
  {
    sf::Vector2f child_position;
    child_position.x = area.left + child_area.x * (i%2);
    child_position.y = area.top + child_area.y * (i >= 2);
      
    children[i] = std::make_shared<node>(node(child_position , child_area));
  }
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
      if(sub_tree->children[i]->area.contains(new_body->get_center()))
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
      if(sub_tree->children[i]->area.contains(new_body->get_center()))
        sub_tree_insert_body(new_body, sub_tree->children[i]);
    for(uint8_t i = 0; i < 4; i++)
      if(sub_tree->children[i]->area.contains(reinsert_body->get_center()))
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

void barnes_hut::draw_rects(sf::RenderWindow& window) const
{
  sf::RectangleShape rect;
  rect.setFillColor(sf::Color::Transparent);
  rect.setOutlineThickness(5e7);
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