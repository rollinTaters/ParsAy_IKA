/*
	MIT License

	Copyright (c) 2025 rollinTaters

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/


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
