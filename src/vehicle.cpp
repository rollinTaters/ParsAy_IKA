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
    m_sensor[0] = Sensor_Emulator( E_type_distance, sf::Vector3f(-0.20, 0,0), sf::Vector3f(0,0,-50) );
    m_sensor[1] = Sensor_Emulator( E_type_distance, sf::Vector3f(-0.10, 0,0), sf::Vector3f(0,0,-20) );
    m_sensor[2] = Sensor_Emulator( E_type_distance, sf::Vector3f( 0.00, 0,0), sf::Vector3f(0,0,  0) );
    m_sensor[3] = Sensor_Emulator( E_type_distance, sf::Vector3f( 0.10, 0,0), sf::Vector3f(0,0, 20) );
    m_sensor[4] = Sensor_Emulator( E_type_distance, sf::Vector3f( 0.20, 0,0), sf::Vector3f(0,0, 50) );
    m_sensor[5] = Sensor_Emulator( E_type_IMU,      sf::Vector3f( 0, 0, 0), sf::Vector3f(0, 0, 0) );

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

// TODO add simulated error to these
sf::Vector3f Vehicle::getVel() const { return m_vel; }
sf::Vector3f Vehicle::getAcc() const { return m_acc; }
sf::Vector3f Vehicle::getAngVel() const { return m_angVel; }
sf::Vector3f Vehicle::getAngAcc() const { return m_angAcc; }

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
    return m_sensor[number].read( m_sensor_data );
}

void Vehicle::setAcceleration( const float acc )
{
    // TODO maybe do a limit check?
    m_acc.y = acc;
}

void Vehicle::setTurnRadius( const float radius )
{
    // TODO maybe do a limit check?
    m_turn_radius = radius;
}

void Vehicle::overridePos( const sf::Vector3f pos )
{
    m_bb3d.setPos(pos);
}

void Vehicle::simulatePhys( const float time_step )
{
    // ah shit, here we go again...
    
    // we calculate new iteration values
    // and set current values to new iterations values
    // we do not want to mix old and new iteration values

    // to avoid mixing
    float fwd_vel = m_vel.y;

    // translations
    m_bb3d.translateLocal( m_vel );

    // rotations, we should switch to quaternions...
    m_bb3d.yawLeft( m_angVel.z );
    m_bb3d.pitchUp( m_angVel.x );
    m_bb3d.rollRight( m_angVel.y );

    // first derivatives (using eulers method)
    m_vel += m_acc * time_step;
    m_angVel += m_angAcc * time_step;

    // second derivatives
    // these are affected by forces, we dont simulate forces. shit.
    // unless we simulate forces, we must model them
    
    // this is centripetal acceleration to model tire sideways friction
    m_acc.x = -(fwd_vel*fwd_vel)/m_turn_radius;
    //m_angAcc = /*DONT HAVE FORCE DATA*/;

    // modelling turning
    m_angVel.z = fwd_vel/m_turn_radius;

    // here be gravity
    /*  TODO we are not ready yet, there is no floor to resist our fall
    Quaternion qc = m_bb3d.getQuat().conjugate();
    sf::Vector3f gravity( 0, 0, -9.81f );
    qc.rotateVector( gravity ); // gravity on local csys
    m_acc += gravity;
    */
}


