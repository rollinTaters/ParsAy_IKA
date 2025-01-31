#pragma once
#include "SFML/Graphics.hpp"

enum gauge_type{
    type1,
    type2
};

class Gauge
{
  public:
    Gauge( gauge_type );

    void updateVal( const float );  // input is value to be displayed
    void updateProportionalVal( const float );  // input is from 0.f to 1.f
    void render();

  private:

    sf::Vector2f m_size;    // in pixels
    sf::Vector2f m_pos;     // of top left corner
    float m_dia;        // diameter in pixels
    
    float m_max_value;
    float m_min_value;
    float m_value;

    // 0 degree is towards top, ex. -90 to 90 will be from left horizon to right horizon
    float m_needle_min_degree;   // angle at which needle rests when min
    float m_needle_max_degree;

    float m_red_start_value;
    float m_red_end_value;

    float m_green_start_value;
    float m_green_end_value;

};
