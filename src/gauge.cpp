#include "gauge.hpp"


///// Gauge /////

Gauge::Gauge( gauge_type gt )
{
    m_size = sf::Vector2f( 200, 200 );
    m_pos = sf::Vector2f( 300, 50 );
    m_dia = 200;

    // TODO write a big switch statement for setting up different gauge types
    switch( gt )
    {
        default:
            m_max_value = 120;
            m_min_value = -10;
            m_value     = 0;

            m_needle_min_degree = -150;   // angle at which needle rests when min
            m_needle_max_degree = 150;

            m_red_start_value   = m_max_value * 0.80f;
            m_red_end_value     = m_max_value;

            m_green_start_value = m_max_value * 0.25f;
            m_green_end_value   = m_max_value * 0.35f;
            break;
        case type1:
            // stuff goes here
            break;
    }
}

void Gauge::updateVal( const float val )
{
    m_value = val;
}

void Gauge::updateProportionalVal( const float prop )
{
    m_value = (prop * (m_max_value-m_min_value)) + m_min_value;
}

void Gauge::render()
{
    // TODO render to global render_target
}


///// Gauge end /////
