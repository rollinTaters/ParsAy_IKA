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


/*
   environment emulator
   - course map (world map)
   - holds real vehicle pos, rotation
   - updates real vehicle pos from motion actuator emulator

   */

// stuff goes here i guess

#pragma once
#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Clock.hpp"
#include "vehicle.hpp"
//#include "sensor_emulator.hpp"  // getSensorData needs to know what a sensor is

class Sensor_Emulator;  // getSensorData needs to know what a sensor is

class Env_Emulator
{
    public:
    Env_Emulator( const Vehicle& );

    // returns time elapsed since emulator start
    sf::Time getTime() const;

    // this returns the emulated sensor readings from the "real" vehicle
    float getSensorData( Sensor_Emulator* ) const;

    private:
    // "real" map/course
    sf::Image m_image_course;
    const unsigned short int m_mm_per_pixel = 5;

    // "real" vehicle, is copied from given vehicle on constructor method
    Vehicle m_real_vehicle;

    // simulation clock
    sf::Clock m_clock;

};

// we have a global environment emulator object, declared at main.cpp
extern Env_Emulator env_emulator;
