

/*
   These are different packets that we use to transfer data between different subsystems
   Feel free to add your own packets if needed
*/
#include <cstdint>

enum Request
{
    request1,
    request2,
    request3
};


// data sent to the drive module
struct Drive_Packet
{
    std::int16_t target_vel;    // (m/s)
    std::int16_t steer_position;    // units
};

// data sent from drive module
struct Drive_Telemetry_Packet
{
    std::int16_t motor_temp1;
    std::int16_t motor_temp2;
    std::int16_t motor_amp1;
    std::int16_t motor_amp2;

    // may be moved to the bms
    std::int16_t battery_voltage;
};


// data sent/received to/from the turret module
struct Turret_Packet
{
    std::int16_t pan;
    std::int16_t tilt;
    bool laser_on;
};


// data sent to the ngc module
struct NGC_Packet
{
    bool autonomous_mode;
};

// from NGC to CCM
struct NGC_Telemetry_Packet
{
    float heading;
    float pitch;
    float yaw;
    // blah blah blah
};


// For testing reasons
struct DPacket
{
    std::uint8_t packet_type;
};


