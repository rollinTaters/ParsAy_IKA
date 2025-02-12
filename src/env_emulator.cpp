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
#include "env_emulator.hpp"

Env_Emulator::Env_Emulator( const Vehicle& inp_vehicle ):
    m_real_vehicle(inp_vehicle)
{
    // load the course image from the disk
    if( !m_image_course.loadFromFile("gfx/course1.png") )
    {
        std::cerr<<"Error. Could not load course image from file!\n";
    }
    // set position of m_real_vehicle in course
    m_real_vehicle.overridePos( sf::Vector3f(340*m_metre_per_pixel, 775*m_metre_per_pixel, 0.5f) );

    // init clock
    m_clock = sf::Clock();
}

sf::Time Env_Emulator::getTime() const
{
    return m_clock.getElapsedTime();
}

bool Env_Emulator::getSensorData( Sensor_Emulator* sensor, Sensor_Data& data ) const
{
    // determine sensor type
    Sensor_Type sensor_type = sensor->getType();

    // determine sensor position
    BB3D sensor_box = sensor->getBox();
    sensor_box += m_real_vehicle.getBox();
    
    // pixel marching setup, for direction sensor
    unsigned int max_iteration = 1000;
    unsigned int iteration = 0;
    bool march_successful = false;
    sf::Vector3f start_pos = sensor_box.getPos();
    start_pos /= m_metre_per_pixel; // convert meters to pixel position
    sf::Vector3f check_pos = start_pos; // position we will iterate upon
    sf::Vector3f direction = sensor_box.getAngEuler();

    switch( sensor_type )
    {
        default:
        case E_type_undefined:
            std::cerr<<"ERROR: Environment emulator got request to read undefined type sensors data\n";
            return false;
        // ---- Simple distance sensor ----
        case E_type_distance:

            while( iteration < max_iteration )
            {
                // check if pixel is marked as "wall"
                if( m_image_course.getPixel( (int)round(check_pos.x),
                                             (int)round(check_pos.y) ) == sf::Color::Black )
                {
                    // found wall, return it
                    march_successful = true;
                    break;
                }
                // TODO what about floors? at least return when point goes below Z0

                // TODO we may skip some pixels if we move by a unit vector, check if this is the case
                // move to next iteration
                check_pos += direction;
            }

            // returning found value
            if( !march_successful )
            {
                // NOTE: maybe there was no obstacle? this isnt necessarly a failure
                std::cerr<<"Environment emulator: sensor did not hit obstacle\n";
                //data.distance = 1000.f;
                return false;
            }
            data.distance = mag( (check_pos - start_pos)*m_metre_per_pixel );
            return true;

        // ---- LIDAR ----
        case E_type_LIDAR:
            // TODO
            std::cerr<<"ERROR: LIDAR type sensor is not implemented in environment emulator\n";
            return false;

        // ---- IMU ----
        case E_type_IMU:
            data.acceleration = m_real_vehicle.getAcc();
            data.angular_rate = m_real_vehicle.getAngAcc();
            // TODO data.magnetic_north;
            data.barometric_pressure = 101325;
            return true;

        // ---- Temperature ----
        case E_type_temperature:
            // TODO
            data.temperature = -273.3;  // ankara
            return false;

        // ---- Electric Current ----
        case E_type_current:
            // TODO
            data.electric_current = 0;
            return false;
    }
}

