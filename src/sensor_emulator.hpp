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
