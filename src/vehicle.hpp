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
   this is our vehicle... 
   thanks captain obvious.


   - has sensor emulators
   - has motion actuator emulators
   */

#pragma once

#include "utility.hpp"
#include "sensor_emulator.hpp"



class Vehicle
{
  public:
    Vehicle();  // constructor
    Vehicle( const Vehicle& );
    Vehicle& operator =( const Vehicle& );

    BB3D getBox() const;    // returns 3d bounding box of vehicle

    Sensor_Emulator getSensor( const unsigned short int number ) const;
    float readSensor( const unsigned short int number );

  private:
    // unless otherwise specified, all 3d vectors are:
    // +X:starboard, +Y:bow, +Z:above, (or width, len, height) (metre)
    // +X:pitch up, +Y:roll right, +Z:yaw left      (degree)

    // sensors on vehicle
    const unsigned short int m_num_sensors = 5;
    Sensor_Emulator m_sensor[5];

    // vehicle geometrical properties, (see utility.hpp)
    BB3D m_bb3d;

    // vehicle physical properties
    sf::Vector3f m_vel;     // velocity vector, (m/s)
    sf::Vector3f m_acc;     // acceleration vector, (m/s^2)
    float m_turn_radius = 0;    // radius of turn circle, positive is towards port, (m)
    float m_mass = 10;          // vehicle mass (kg)
    //sf::Vector3f m_moment_of_inertia;
    //sf::Vector3f m_center_of_mass;

};

