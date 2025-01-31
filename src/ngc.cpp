#include "ngc.hpp"

// constructor
NGC::NGC( Vehicle* vehicle )
{
    // init internal world map
    int internal_map_size = 40; // metre
    m_image_world_map.create( internal_map_size * 1000 / m_mm_per_pixel,
                              internal_map_size * 1000 / m_mm_per_pixel );

    // assign controlled vehicle
    m_vehicle = vehicle;

};


bool NGC::deadReckon()
{
    return false;
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
