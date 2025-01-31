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

