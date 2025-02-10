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


#include <iostream>     // cerr
#include "vehicle.hpp"
#include "env_emulator.hpp" // read sensor

Vehicle::Vehicle()
{
    // populate sensors on vehicle
    m_sensor[0] = Sensor_Emulator( sf::Vector3f(-0.20, 0,0), sf::Vector3f(0,0,-50) );
    m_sensor[1] = Sensor_Emulator( sf::Vector3f(-0.10, 0,0), sf::Vector3f(0,0,-20) );
    m_sensor[2] = Sensor_Emulator( sf::Vector3f( 0.00, 0,0), sf::Vector3f(0,0,  0) );
    m_sensor[3] = Sensor_Emulator( sf::Vector3f( 0.10, 0,0), sf::Vector3f(0,0, 20) );
    m_sensor[4] = Sensor_Emulator( sf::Vector3f( 0.20, 0,0), sf::Vector3f(0,0, 50) );

    m_vel = sf::Vector3f( 0,0,0 );
    m_acc = sf::Vector3f( 0,0,0 );

    m_bb3d.setSize(0.8, 1.3, 0.6);
    m_bb3d.setPos(0,0,0);
    m_bb3d.setAng(0,0,0);
}

Vehicle::Vehicle( const Vehicle& other ):
    m_num_sensors(other.m_num_sensors)
{
    for( int i = 0; i < m_num_sensors; i++ )
    {
        m_sensor[i] = other.m_sensor[i];
    }
    m_bb3d = other.m_bb3d;
    m_vel = other.m_vel;
    m_acc = other.m_acc;
    m_turn_radius = other.m_turn_radius;
    m_mass = other.m_mass;
}

Vehicle& Vehicle::operator =( const Vehicle& rhs )
{
    return *this = Vehicle(rhs);
}

BB3D Vehicle::getBox() const { return m_bb3d; }

Sensor_Emulator Vehicle::getSensor( const unsigned short int number ) const
{
    if( number >= m_num_sensors )
    {
        std::cerr<<"ERROR! Requested non existing sensor, returning default constructed sensor.\n";
        return Sensor_Emulator();
    }
    return m_sensor[ number ];
}

float Vehicle::readSensor( const unsigned short int number )
{
    if( number >= m_num_sensors )
    {
        std::cerr<<"ERROR! Requested read on non existing sensor, returning 0 as read value.\n";
        return 0.f;
    }
    return m_sensor[number].read();
}

