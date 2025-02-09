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
   sensor emulator:
   - using the environment emulator;
   - models sensor read errors, angular and positional deviations
   - returns would be read value

   */

#pragma once
#include "utility.hpp"
#include "SFML/System/Time.hpp"

class Sensor_Emulator
{
  public:
    Sensor_Emulator();  // DO NOT use, here to shut up the compiler

    Sensor_Emulator( const sf::Vector3f inp_pos,
                     const sf::Vector3f inp_angle );

    Sensor_Emulator( const BB3D inp_bb3d );

    bool isReady() const;
    float read();

  private:

    // position info, relative to the vehicle
    BB3D m_bb3d;

    // these simulate if the sensor is ready for next read
    sf::Time m_time_last_read;
    sf::Time m_time_cooldown;

};
