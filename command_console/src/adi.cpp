/*
    MIT License

    Copyright (c) 2025 rollinTaters, guvenchemy

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

/// Attitude Director Indicator /// 

#include "adi.hpp"

Adi::Adi(sf::Vector2f pos, float dia){
        m_dia = dia;
        m_pos = pos;

        m_roll_value = 0;
        m_max_roll_value = 180;
        m_min_roll_value = -180;

        m_pitch_value = 0;
        m_max_pitch_value = 100;
        m_min_pitch_value = 200;
        // uploading textures
        if (!roll_markings.loadFromFile("./assets/attitude_director_indicator/roll_markings_.png")) {
            std::cerr << "Error: adi class could not load Roll markings texture!";
        }
        if (!pitch_scale.loadFromFile("./assets/attitude_director_indicator/pitch_scale_.png")) {
            std::cerr << "Error: adi class could not load Pitch scale texture!";
        }
        if (!horizon.loadFromFile("./assets/attitude_director_indicator/horizon.png")) {
            std::cerr << "Error: adi class could not load Horizon texture!";
        }
        if (!horizon_outline.loadFromFile("../assets/attitude_director_indicator/horizon.png")) {
            std::cerr << "Error: adi class could not load Horizon texture!";
        }

        roll_markings_sprite.setTexture(roll_markings);
        pitch_scale_sprite.setTexture(pitch_scale);
        horizon_sprite.setTexture(horizon);

      
        // Calculate scale factors to match the gauge size
        // our textures are 600x600
        roll_markings_sprite.setScale( dia/600, dia/600 );
        pitch_scale_sprite.setScale( dia/600, dia/600 );
        horizon_sprite.setScale( dia/600 , dia/600 );
       
        // Set origin to center of texture
        roll_markings_sprite.setOrigin(roll_markings.getSize().x / 2.f, roll_markings.getSize().y / 2.f);
        pitch_scale_sprite.setOrigin(pitch_scale.getSize().x / 2.f, pitch_scale.getSize().y / 2.f);
        horizon_sprite.setOrigin(horizon.getSize().x / 2.f, horizon.getSize().y / 2.f);
      
        
        // Position at center of gauge
        roll_markings_sprite.setPosition(m_pos.x , m_pos.y + m_dia / 2.f);
        pitch_scale_sprite.setPosition(m_pos.x , m_pos.y + m_dia / 2.f);
        horizon_sprite.setPosition(m_pos.x , m_pos.y + m_dia / 2.f);
       

        horizon_sprite.setRotation(0);
        pitch_scale_sprite.setRotation(0);

        m_background.setFillColor(sf::Color::Transparent);
        m_background.setRadius(m_dia/2);
        m_background.setOrigin(m_dia/2,m_dia/2);
        m_background.setScale(m_dia/500, m_dia/500);
        m_background.setPosition(m_pos.x , m_pos.y + m_dia / 2.f);
        m_background.setOutlineThickness(500);
        m_background.setOutlineColor(sf::Color(180,180,180));


}
void Adi::updateRollVal_prop(const float prop)
{
    m_roll_value = (prop * (m_max_roll_value - m_min_roll_value)) + m_min_roll_value;
}
void Adi::updatePitchVal_prop(const float prop)
{
    m_pitch_value = (prop * (m_max_pitch_value - m_min_pitch_value)) + m_min_pitch_value;
}

void Adi::updateRollVal(const float val)
{
    m_roll_value = val;

    if (m_roll_value < m_min_roll_value)
        m_roll_value = m_min_roll_value;
    if (m_roll_value > m_max_roll_value)
        m_roll_value = m_max_roll_value;
}

void Adi::updatePitchVal(const float val)
{
    m_pitch_value = val;

    if (m_pitch_value < m_min_pitch_value)
        m_pitch_value = m_min_pitch_value;
    if (m_pitch_value > m_max_pitch_value)
        m_pitch_value = m_max_pitch_value;
}

void Adi::render(sf::RenderTarget& target){
        // Safely check if textures are loaded before trying to draw sprites     

        sf::Vector2f originalPosition(m_pos.x , m_pos.y + m_dia / 2.f); 
        float newX = originalPosition.x;
        float newY = originalPosition.y - m_pitch_value * m_pitch_scale;
        
        float x_min = m_pos.x - m_dia / 2.f;
        float x_max = m_pos.x + m_dia / 2.f;
        float y_min = m_pos.y - m_dia / 2.f;
        float y_max = m_pos.y + m_dia / 2.f;
    
        if (newX < x_min) newX = x_min;
        if (newX > x_max) newX = x_max;
        if (newY < y_min) newY = y_min;
        if (newY > y_max) newY = y_max;
    

        horizon_sprite.setRotation(m_roll_value);
        pitch_scale_sprite.setRotation(m_roll_value);

        horizon_sprite.setPosition(newX, newY);
        pitch_scale_sprite.setPosition(newX, newY);

        // Draw ADI components in correct order (after updating their pos and rotations)
        target.draw(horizon_sprite);
       
        target.draw(pitch_scale_sprite);
        target.draw(m_background); 
        target.draw(roll_markings_sprite);
        
}
