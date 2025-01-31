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
