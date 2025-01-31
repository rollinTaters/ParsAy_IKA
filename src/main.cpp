/*
   motion actuators emulator:
   - steer actuators
     models turn radious, (max and min radious too)
   - drive actuators
     models current linear speed, accel, inertia
   - emulates potential errors (wheel slip, random disturbances)



*/
#include <iostream>

#include "env_emulator.hpp"
#include "sensor_emulator.hpp"
#include "ngc.hpp"

// this is the vehicle we are managing/controlling
Vehicle simulated_vehicle;

// our global environment emulator, if you included env_emulator.hpp, you know about its existance
Env_Emulator env_emulator( simulated_vehicle );

int main()
{
    std::cout<<"ULV NGC Emulator v0.1\n";

    // our navigation guidance and control system
    NGC ngc_system( &simulated_vehicle );

    std::cout<<"Exiting. Have a nice day\n";
}
