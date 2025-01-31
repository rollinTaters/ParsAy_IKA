/*
   control code (aka the vehicle)
   - handles navigation, guidance, control (NGC)
   - has internal world map (at least prev vehicle positions, and possibly more data such as stage positions, target positions)
   - has internal "surroundings" map, this is volatile (short term and overwritable?)
     is not used for "mapping", only used for immediate obstacle avoidance


   */

#pragma once

#include <vector>
#include "vehicle.hpp"
#include "SFML/Graphics/Image.hpp"

class NGC
{
  public:
    // upon NGC code start, vehicle is assumed to be at 0,0 world position
    NGC( Vehicle* vehicle );

  private:

    // internal world map
    sf::Image m_image_world_map;
    const unsigned short int m_mm_per_pixel = 5;

    // controlled vehicle
    Vehicle* m_vehicle = nullptr;

    // immediate surrounding obstacles
    // this will most likely be current sensor readings
    std::vector<sf::Vector3f> m_immediate_obstacles;

    // dead reckon function: ( NAVIGATION )
    // - using the IMU data, plot previous positions on internal world map
    bool deadReckon();

    // obstacle mark function: ( NAVIGATION )
    // - using the LIDAR data mark obstacles on internal world map
    //  this adds them to the world map as permanent obstacles
    //  is used for mapping an area
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
    bool createOpenSpaceWaypoint();

};
