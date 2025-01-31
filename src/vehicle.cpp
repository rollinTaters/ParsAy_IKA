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

