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


class Gauge
{
public:
    enum gauge_type {
        type_temperature,
        type_amp,
        type_adi,
        type_compass
    };

    Gauge(gauge_type type, sf::Vector2f pos, float dia);

    void updateVal(const float);  // input is value to be displayed
    void updateProportionalVal(const float);  // input is from 0.f to 1.f
    void render(sf::RenderTarget& target);
    void setupGaugeFrame();
    void setupNeedle();

private:
    gauge_type m_type; // to store gauge type
    // adi things
    sf::Texture roll_markings; // roll markings
    sf::Texture pitch_scale; // pitch scale
    sf::Texture horizon; // horizon
    sf::Sprite roll_markings_sprite;
    sf::Sprite pitch_scale_sprite;
    sf::Sprite horizon_sprite;

    // compass things
    sf::Texture compass_bg; // compass background
    sf::Texture compass_ticks_numbers; // compass ticks and numbers
    sf::Sprite compass_bg_sprite;
    sf::Sprite compass_ticks_numbers_sprite;



    // ampere and temperature meter things 
    sf::CircleShape m_gaugeFrame;    // gauge shape
    
    std::vector<sf::Vertex> m_ticks; // lines
    std::vector<sf::Text> m_numbers; // numbers in gauge
    sf::Font m_font;          // font type
    
    sf::Vector2f m_size;    // in pixels
    sf::Vector2f m_pos;     // of top left corner
	
    sf::CircleShape m_center;        // center dot of gauge
    sf::RectangleShape m_needle; // needle 
	
    sf::Text m_label; // temperature - amp 
    std::string m_label_text;

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
