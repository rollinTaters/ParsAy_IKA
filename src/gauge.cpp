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


#include "gauge.hpp"

///// Gauge /////

Gauge::Gauge(gauge_type gt, sf::Vector2f pos, float dia)
{
    m_size = sf::Vector2f(dia, dia);
    m_pos = pos;
    m_dia = dia;


    // setting gauge frame.
    m_gaugeFrame.setRadius(m_dia / 2);
    m_gaugeFrame.setFillColor(sf::Color::White);
    m_gaugeFrame.setOutlineThickness(5);
    m_gaugeFrame.setOutlineColor(sf::Color::Black);
    m_gaugeFrame.setOrigin(m_dia / 2, m_dia / 2);
    m_gaugeFrame.setPosition(m_pos.x + m_dia / 2, m_pos.y + m_dia / 2);

    // uploading Font   
    if (!m_font.loadFromFile("./assets/fonts/arial.ttf")) {
        std::cerr << "Error: gauge class could not load font\n";
    }

    // TODO write a big switch statement for setting up different gauge types
    switch (gt)
    {
    default:
        m_max_value = 120;
        m_min_value = -10;
        m_value = 0;

        m_needle_min_degree = -150;   // angle at which needle rests when min
        m_needle_max_degree = 150;

        m_red_start_value = m_max_value * 0.80f;
        m_red_end_value = m_max_value;

        m_green_start_value = m_max_value * 0.25f;
        m_green_end_value = m_max_value * 0.35f;

        for (int i = 0; i <= 20; i++) {
            // creating gauge marks
            float angle = (i * 15 + m_needle_min_degree) * PI / 180.0f;
            float outerRadius = m_dia / 2;
            float innerRadius = (i % 2 == 0) ? outerRadius - 20 : outerRadius - 10;

            sf::Vector2f center(m_pos.x + m_dia / 2, m_pos.y + m_dia / 2);
            sf::Vector2f outerPoint(center.x + outerRadius * cos(angle),
                center.y + outerRadius * sin(angle));
            sf::Vector2f innerPoint(center.x + innerRadius * cos(angle),
                center.y + innerRadius * sin(angle));

            m_ticks.push_back(sf::Vertex(outerPoint, sf::Color::Black));
            m_ticks.push_back(sf::Vertex(innerPoint, sf::Color::Black));

            // creating numbers
            float textRadius = (m_dia / 2) - 30;

            if (i % 2 == 0) {
                sf::Text text(std::to_string(i*10), m_font, 16);
                text.setFillColor(i <= 6 ? sf::Color::Black : (i <= 10 ? sf::Color::Yellow : sf::Color::Red));
                // getting text bounds to center text
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin(textBounds.width / 2, textBounds.height / 2);
                // get new calculated position
                sf::Vector2f textPos(center.x + textRadius * cos(angle),
                    center.y + textRadius * sin(angle));
                text.setPosition(textPos);
                m_numbers.push_back(text);
            }
        }

        break;
    case type_temperature:
		m_label_text = "temp C: ";
        m_label = sf::Text(m_label_text + std::to_string(m_value), m_font, 14);

        m_max_value = 120;
		m_min_value = -5;
        
        m_value = 0;

        m_needle_min_degree = -150;   // angle at which needle rests when min
        m_needle_max_degree = 150;

        m_red_start_value = m_max_value * 0.80f;
        m_red_end_value = m_max_value;

        m_green_start_value = m_max_value * 0.25f;
        m_green_end_value = m_max_value * 0.35f;

        for (int i = 0; i <= 20; i++) {
            // creating gauge marks
            float angle = (i * 15 + m_needle_min_degree) * PI / 180.0f;
            float outerRadius = m_dia / 2;
            float innerRadius = (i % 2 == 0) ? outerRadius - 20 : outerRadius - 10;

            sf::Vector2f center(m_pos.x + m_dia / 2, m_pos.y + m_dia / 2);
            sf::Vector2f outerPoint(center.x + outerRadius * cos(angle),
                center.y + outerRadius * sin(angle));
            sf::Vector2f innerPoint(center.x + innerRadius * cos(angle),
                center.y + innerRadius * sin(angle));

            m_ticks.push_back(sf::Vertex(outerPoint, sf::Color::Black));
            m_ticks.push_back(sf::Vertex(innerPoint, sf::Color::Black));

            // creating numbers
            float textRadius = (m_dia / 2) - 30;

            if (i % 2 == 0) {
                double value = (i * 6.25) - 5;
                std::string textString = std::to_string(value);
				// Erasing trailing zeros
                textString.erase(textString.find_last_not_of('0') + 1, std::string::npos);
                if (textString.back() == '.') textString.pop_back();

                sf::Text text(textString, m_font, 14);
                text.setFillColor(i <= 6 ? sf::Color::Black : (i <= 10 ? sf::Color::Yellow : sf::Color::Red));
                // getting text bounds to center text
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin(textBounds.width / 2, textBounds.height / 2);
                // get new calculated position
                sf::Vector2f textPos(center.x + textRadius * cos(angle),
                    center.y + textRadius * sin(angle));
                text.setPosition(textPos);
                m_numbers.push_back(text);
            }
        }
        break;
	case type_amp:
		m_label_text = "current A: ";
        m_label = sf::Text(m_label_text + std::to_string(m_value), m_font, 14);
		
        m_max_value = 40;
		m_min_value = 0;
        
        m_value = 0;

        m_needle_min_degree = -150;   // angle at which needle rests when min
        m_needle_max_degree = 150;

        m_red_start_value = m_max_value * 0.80f;
        m_red_end_value = m_max_value;

        m_green_start_value = m_max_value * 0.25f;
        m_green_end_value = m_max_value * 0.35f;

        for (int i = 0; i <= 20; i++) {
            // creating gauge marks
            float angle = (i * 15 + m_needle_min_degree) * PI / 180.0f;
            float outerRadius = m_dia / 2;
            float innerRadius = (i % 2 == 0) ? outerRadius - 20 : outerRadius - 10;

            sf::Vector2f center(m_pos.x + m_dia / 2, m_pos.y + m_dia / 2);
            sf::Vector2f outerPoint(center.x + outerRadius * cos(angle),
                center.y + outerRadius * sin(angle));
            sf::Vector2f innerPoint(center.x + innerRadius * cos(angle),
                center.y + innerRadius * sin(angle));

            m_ticks.push_back(sf::Vertex(outerPoint, sf::Color::Black));
            m_ticks.push_back(sf::Vertex(innerPoint, sf::Color::Black));

            // creating numbers
            float textRadius = (m_dia / 2) - 30;

            if (i % 2 == 0) {
                sf::Text text(std::to_string(i*2), m_font, 14);
                text.setFillColor(i <= 6 ? sf::Color::Black : (i <= 10 ? sf::Color::Yellow : sf::Color::Red));
                // getting text bounds to center text
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin(textBounds.width / 2, textBounds.height / 2);
                // get new calculated position
                sf::Vector2f textPos(center.x + textRadius * cos(angle),
                    center.y + textRadius * sin(angle));
                text.setPosition(textPos);
                m_numbers.push_back(text);
            }
        }
        break;
    }

    // creating needle 
    m_needle.setSize(sf::Vector2f(m_dia /2, 1.5)); // needle size
    m_needle.setFillColor(sf::Color::Red);
    m_needle.setOrigin(0 , .75f); // making left end the rotation point
    m_needle.setPosition(m_pos.x + m_dia / 2, m_pos.y + m_dia / 2); // center of gauge
    m_needle.setRotation(m_needle_min_degree); // starting point.
	//std::cout << "Needle Position: " << m_needle.getPosition().x << ", " << m_needle.getPosition().y << std::endl; // for debugging
    // center dot
	m_center.setRadius(5);
	m_center.setFillColor(sf::Color::Black);
	m_center.setOrigin(5, 5);
	m_center.setPosition(m_pos.x + m_dia / 2, m_pos.y + m_dia / 2);

	// label
	m_label.setPosition(m_pos.x + m_dia / 2 - 50, m_pos.y + m_dia + 10);
	m_label.setFillColor(sf::Color::Black);
	m_label.setFont(m_font);
    

    
}

void Gauge::updateVal(const float val)
{
        m_value = val;

}


void Gauge::updateProportionalVal(const float prop)
{
    m_value = (prop * (m_max_value - m_min_value)) + m_min_value;
}

void Gauge::render(sf::RenderTarget& target)
{
    target.draw(m_gaugeFrame);
    target.draw(m_ticks.data(), m_ticks.size(), sf::Lines);
    for (const auto& num : m_numbers) {
        target.draw(num);
    }
    std::string value = std::to_string(m_value);
    value.erase(value.find('.') + 3);
    if (value.back() == '.') value.pop_back();
    m_label.setString(m_label_text + value);
	target.draw(m_label);
	// needle rotation
	float needleAngle = m_needle_min_degree + (m_value - m_min_value) * (m_needle_max_degree - m_needle_min_degree) / (m_max_value - m_min_value);
    m_needle.setRotation(needleAngle);
    target.draw(m_needle);
	target.draw(m_center);

	//std::cout << "m_value: " << m_value << " -> Needle Angle: " << needleAngle << std::endl; // for debugging
    // by adding sf::RenderTarget% target parameter i tried to provide some flexibility
}


///// Gauge end /////
