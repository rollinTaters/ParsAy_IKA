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


#include <chrono>
#include <iostream> // cout cerr
#include "ngc.hpp"

// == public: ==

NGC::NGC( Vehicle* vehicle )
{
    // init internal world map
    int internal_map_size = 40; // metre
    m_image_world_map.create( internal_map_size / m_metre_per_pixel,
                              internal_map_size / m_metre_per_pixel );

    // assign controlled vehicle
    m_vehicle = vehicle;

};

NGC::~NGC()
{
    stop();
    stopDeadReckoning();
}

bool NGC::start()
{
    // start necessary threads and do runtime initializations

    // check if we are already runnng the thread
    if( m_main_thread != nullptr )
    {
        std::cerr<<"NGC: Warning: Tried to start main thread when it is already running.\n";
        return false;
    }
    std::cout<<"NGC: Starting main thread\n";
    m_run_main_thread = true;
    m_main_thread = new std::thread( &NGC::mainThreadFunc, this );
    return true;
}

bool NGC::stop()
{
    // send stop signals and join the threads here

    // check if we have a thread to stop
    if( m_main_thread == nullptr )
    {
        std::cerr<<"NGC: Warning: Tried to stop main thread. We dont have a main thread to stop.\n";
        return false;
    }
    std::cout<<"NGC: Stopping main thread\n";
    // signal main thread to stop
    m_run_main_thread = false;
    m_main_thread->join();
    delete m_main_thread;
    m_main_thread = nullptr;

    // TODO is there a fail condition?? can the thread not join? what then?
    return true;
}

bool NGC::startDeadReckoning()
{
    m_last_dead_reckon_time = m_clock.now();
    if( m_dead_reckoning_thread != nullptr )
    {
        //std::cerr<<"NGC: Warning: Dead reckoning thread is already running.\n";
        return false;
    }
    m_run_dead_reckoning_thread = true;
    m_dead_reckoning_thread = new std::thread( &NGC::deadReckonFunc, this );
    return true;
}

bool NGC::stopDeadReckoning()
{
    if( m_dead_reckoning_thread == nullptr )
    {
        //std::cerr<<"NGC: Warning: Tried to stop dead reckoning thread, there is no thread.\n";
        return false;
    }
    m_run_dead_reckoning_thread = false;
    m_dead_reckoning_thread->join();
    delete m_dead_reckoning_thread;
    m_dead_reckoning_thread = nullptr;
    return true;
}

// == private: ==

void NGC::mainThreadFunc()
{
    // on first start, check if dead reckoning is active
    if( startDeadReckoning() )
        std::cout<<"Starting dead reckoning\n";

    int counter = 0;
    while( m_run_main_thread )
    {
        // roll rol roll
        // TODO check if dead reckoning is still active??

        // TODO run predictTrajectory and send it to command console for debug visualization

        // TODO read lidar and run markImmediateObstacles

        // TODO run createTargetWaypoint, createOpenSpaceWaypoint

        // TODO run CONTROL type methods

        std::cout<<"ngc main thread spam. counter:"<<counter<<"\n";
        counter++;
        std::this_thread::sleep_for( std::chrono::milliseconds(200) );
    }
    // DEBUG
    std::cout<<"NGC: Main thread exiting\n";
}

bool NGC::getLIDARData()
{
    // since we dont actually have a vehicle, or comminucations system determined
    // just ask "m_vehicle" for readSensor
    return false;
}

void NGC::deadReckonFunc()
{
    using namespace std::chrono;
    std::chrono::milliseconds time_step;
    while( m_run_dead_reckoning_thread )
    {
        // check if its time to do it
        if( m_clock.now() < m_last_dead_reckon_time + m_dead_reckoning_interval )
        {
            // nope, not now
            std::this_thread::sleep_for( m_dead_reckoning_interval /5 );
            continue;
        }else{
            time_step = duration_cast<milliseconds>(steady_clock::now() - m_last_dead_reckon_time);
        }

        // make sure the vehicles internal IMU sensor data is up to date
        // NOTE: sensor[0] is always the IMU (i just decided that)
        // request IMU sensor read
        m_vehicle->readSensor(0);

        // immediately update time point
        m_last_dead_reckon_time = m_clock.now();

        // run vehicles setNavigationState, which actually does the dead reckoning calculations
        m_vehicle->setNavigationState( time_step.count() );

        // TODO this time step should be reported as a quality measure to the command console
    }
}

bool NGC::markObstacles()
{
    return false;
}

bool NGC::markImmediateObstacles()
{
    return false;
}

bool NGC::predictTrajectory()
{
    return false;
}

bool NGC::createTargetWaypoint()
{
    return false;
}

bool NGC::createOpenSpaceWaypoint()
{
    return false;
}
