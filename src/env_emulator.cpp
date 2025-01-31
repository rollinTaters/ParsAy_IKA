#include "env_emulator.hpp"

Env_Emulator::Env_Emulator( const Vehicle& inp_vehicle ):
    m_real_vehicle(inp_vehicle)
{
    // constructor
    // TODO maybe load the course image from the disk?

    // init clock
    m_clock = sf::Clock();
}

sf::Time Env_Emulator::getTime() const
{
    return m_clock.getElapsedTime();
}

float Env_Emulator::getSensorData( Sensor_Emulator* sensor ) const
{
    // TODO read from m_real_vehicle
    return 0.f;

    // determine sensor position
}

