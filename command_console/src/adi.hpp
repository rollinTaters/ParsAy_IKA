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
#pragma once
#include "SFML/Graphics.hpp"
#include <cmath>
#include <iostream>
#define PI 3.141592

class Adi 
{
    public:
        Adi(sf::Vector2f pos, float dia);
        void updateRollVal_prop(const float);   // 0-1
        void updatePitchVal_prop(const float);  // 0-1
        void updateRollVal(const float);    // degree
        void updatePitchVal(const float);   // degree
        void render(sf::RenderTarget& target);

    private:
        // adi things
        sf::Texture roll_markings; // roll markings
        sf::Texture pitch_scale; // pitch scale
        sf::Texture horizon; // horizon
        
        sf::Texture horizon_outline;

        //sf::Sprite horizon_outline_sprite;
        
        sf::Sprite roll_markings_sprite;
        sf::Sprite pitch_scale_sprite;
        sf::Sprite horizon_sprite;

        sf::CircleShape m_background;
    
        float m_dia;        // diameter in pixels

        float m_max_roll_value;
        float m_min_roll_value;
        float m_roll_value;

        float m_max_pitch_value;
        float m_min_pitch_value;
        float m_pitch_value;
        const float m_pitch_scale = 0.5f; //FIXME this needs tuning

        sf::Vector2f m_size;    // in pixels
        sf::Vector2f m_pos;     // of top left corner    

};
