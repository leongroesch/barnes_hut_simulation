#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <iostream>
#include <functional>

class user_interface
{
  private:
    sf::RenderWindow window;
    tgui::Gui gui;
    tgui::Grid::Ptr grid_layout;

    tgui::Button::Ptr start_button; 
    tgui::TextBox::Ptr num_body_tb;
    tgui::TextBox::Ptr max_mass_tb;
    tgui::TextBox::Ptr min_mass_tb;

    std::function<void(float, float, float) > start_callback;

    static constexpr int textbox_width{70};
    
    void start_pressed() const;

  public:
    user_interface(sf::VideoMode vidoe_mode, std::function<void(float, float, float)> start_callback);

    void handle_event();
    void draw();

    // std::vector<float> get_settings() const;

};

#endif