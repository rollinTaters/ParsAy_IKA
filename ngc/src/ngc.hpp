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
   control code (aka the vehicle)
   - handles navigation, guidance, control (NGC)
   - has internal world map (at least prev vehicle positions, and possibly more data such as stage positions, target positions)
   - has internal "surroundings" map, this is volatile (short term and overwritable?)
     is not used for "mapping", only used for immediate obstacle avoidance


   */

#pragma once

#include <thread>
#include <vector>
#include <vector>
#include "vehicle.hpp"
#include "traction_motor.hpp"
#include "../../common_code/src/comms_module.hpp"
#include "../../common_code/src/utility.hpp"
#include "SFML/Graphics/Image.hpp"

class NGC
{
  public:
    // upon NGC code start, vehicle is assumed to be at 0,0 world position
    NGC( Vehicle* vehicle );
    ~NGC();

    NGC( const NGC& )  = delete;
    NGC( const NGC&& ) = delete;

    bool start();
    bool stop();

    bool startDeadReckoning();
    bool stopDeadReckoning();

    bool addWP( Point );    // add new waypoint to the queues end
    bool addWP( Point, int );   // add it after given slot
    std::vector<Point> getWPs() const;
    bool executeWPs();  // starts executing current waypoints

  private:

    // ==== Data ====
    /*
    // NOTE 2d images are cool for debugging, but we want to switch to 3d world data
    // internal world map
    sf::Image m_image_world_map;    // DEPRECATED
    const float m_metre_per_pixel = 0.005;  // 5mm per pixel
    */


    // waypoints
    std::vector<Point> m_waypoints;
    bool m_execute_waypoints = false;

    // time keeping and clocks
    std::chrono::steady_clock m_clock;
    std::chrono::milliseconds m_dead_reckoning_interval = std::chrono::milliseconds(50);
    std::chrono::time_point<std::chrono::steady_clock> m_last_dead_reckon_time;

    // controlled vehicle
    Vehicle* m_vehicle = nullptr;
    TractionMotor motor_R;
    TractionMotor motor_L;

    // immediate surrounding obstacles
    // this will most likely be current sensor readings
    std::vector<Point> m_immediate_obstacles;

    // thread control
    bool m_run_main_thread = false;
    bool m_run_dead_reckoning_thread = false;
    std::thread *m_main_thread = nullptr;
    std::thread *m_dead_reckoning_thread = nullptr;

    // communications module
    CommsModule m_comms_module;
    NGC_Command_Packet m_command_packet;

    

    // ==== Methods ====

    // main thread runs this
    void mainThreadFunc();

    // ask the lidar u-controller for fresh data
    bool getLIDARData();

    // dead reckon function/thread: ( NAVIGATION )
    // - using the IMU data, plot previous positions on internal world map
    void deadReckonFunc();

    // obstacle mark function: ( NAVIGATION )
    // - using the LIDAR data mark obstacles on internal world map
    //  this adds them to the world map as permanent obstacles
    //  is used for mapping an area (aka SLAM)
    bool markObstacles();

    // immediate obstacle mark function: ( NAVIGATION )
    // - using the LIDAR data mark obstacles on the "surroundings" map
    //  is used for collision avoidance
    bool markImmediateObstacles();

    // predict trajectory function: ( NAVIGATION )
    // - using vehicle steer actuator, drive actuator, and inertia, predicts next x number of positions in y amout of time
    bool predictTrajectory();

    // create target waypoint function: ( GUIDANCE )
    // - look at the previous positions on internal world map,
    //  find a new position which is "in short range", "unexplored" and "reachable"
    //  set it as a target waypoint
    bool createTargetWaypoint();

    // create "open space" waypoint function: ( GUIDANCE )
    // - this may be used to modify an existing waypoint,
    //  or created and then "merged" with an existing waypoint to modify it
    // - takes an input position and finds a new position which is furthest away from any obstacles, but closest to input pos
    bool createOpenSpaceWaypoint( Point& );


    bool hitWP( Point );

    // navigation happens with a queue of waypoints, sub waypoints may need to be calculated for this queueueueu

    // Control modes with waypoints
    // (mode 1) find attitude to reach target wp
    // (mode 2) (interpolated wp) calculate intermediate wp that when used, makes us reach target wp with desired attitude
    // (mode 3) (immediate after wp) use mode1 then add new wp immediately after it to match a given attitude

    // Waypoint usage modes
    // (mode 1) hit waypoint
    // (mode 2-3) hit wp with attitude
    // (mode 4) wp is apex of turn, calculate turn arc using prev and next wp, these points may be used as sub-waypoints
    // (mode 5) wp is arc center of turn, rest is same as mode4

    // == Control Methods ==
    void halt();
    void setControlOutput_rate( float, float );
    void setControlOutput_radius( float, float );
    
};



