/*
    MIT License

    Copyright (c) 2025 rollinTaters, guvenchemy

*/

#include "user_input.hpp"
#include <raylib.h>
#include <iostream>
#include "../../common_code/src/comms_module.hpp"


UserInput::UserInput()
{
}

void UserInput::switchDriveMode( DriveMode newMode )
{
    m_drive_mode = newMode;
}

void UserInput::processInput( CommsPacket &dcp )
{

    /*
    FIXME there are two identifier things about comms modules:
    - one is where you send the packet to:
        comms_module.sendPacket( XX, CommsModule::ngc_channel );
        comms_module.sendPacket( XX, CommsModule::console_channel );
        comms_module.sendPacket( XX, CommsModule::drive_channel );
        comms_module.sendPacket( XX, CommsModule::turret_channel );

    - the other is what type of packet you send:
        CommsPacket packet1( CommsPacket::drive_command );
        CommsPacket packet1( CommsPacket::drive_telemetry );
        CommsPacket packet1( CommsPacket::ngc_command );
        CommsPacket packet1( CommsPacket::turret );
        CommsPacket packet1( CommsPacket::video_packet );

    So...
    when you call this process input method, what king of packet will you send?
    and to where will you send it?
    ( you probably always want to send it to Central Command Module (CCM) )

    for now, for testing purposes, this packet is modified to be a ngc_command packet
    */
    dcp.packet_type = CommsPacket::ngc_command;

    // XXX
    // TODO grab all the commands from ngc/gui.hpp and implement them here
    // XXX

    // populate the given packet with the input read from the console
    static float speed = 0.0f;  
    const float speed_increment = 0.05f; 
    const float max_speed = 5.0f; 
    const float min_speed = 0.0f;

    static float rate = 0.0f;  
    const float rate_increment = 0.10f; 
    const float rate_dead_zone = 0.12f; // note: this should be bigger than increment, otherwise expect bugs
    const float max_rate =  1.0f; 
    const float min_rate = -1.0f;

    if (IsGamepadAvailable(0)) 
    {
        float leftX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float leftY = -GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y); // axis is inverted
        
        // TODO show the state of commands issued on the screen,
        // we wanna know what kind of manual command we sent

        // Small joystick movements are ignored to ensure stable controls
        if (leftX > 0.1f) { // Creating deadzone because we don't want our little vehicle to shake
            //std::cout << "Move right" << std::endl;
            dcp.setManualSteer(leftX); 
        } else if (leftX < -0.1f) {
            //std::cout << "Move left" << std::endl;   
            dcp.setManualSteer(leftX); 
        } else {
            dcp.setManualSteer( 0 );
        }

        if (leftY > 0.1f) {
            //std::cout << "Move forward" << std::endl;  
            dcp.setManualSpeed(leftY);  
        } else if (leftY < -0.1f) {
            //std::cout << "Move backward" << std::endl; 
            dcp.setManualSpeed(leftY); 
        } else {
            dcp.setManualSpeed( 0 );
        }
    }
    else
    {
        // Handle speed increase when UP key is pressed
        if (IsKeyDown(KEY_UP)) {
            speed += speed_increment;  // Increase speed
            if (speed > max_speed) speed = max_speed;  // Cap speed to max value
            //std::cout << "Speed increasing: " << speed << std::endl;
        }
        else if (IsKeyDown(KEY_DOWN)) {
            speed -= speed_increment;  // Decrease speed
            if (speed < -max_speed) speed = -max_speed; // Cap speed to max negative value
            //std::cout << "Speed decreasing: " << speed << std::endl;
        } else {
            if (speed <= min_speed)
                speed = min_speed;
            else
                speed -= speed_increment; 
        }

        // Handle steering with LEFT/RIGHT keys
        if (IsKeyDown(KEY_LEFT)) {
            rate += rate_increment;
            //std::cout << "Turn right" << std::endl;
        } else if (IsKeyDown(KEY_RIGHT)) {
            rate -= rate_increment;
            //std::cout << "Turn left" << std::endl;
        } else {
            if( rate >= rate_dead_zone )
                rate -= rate_increment;
            else if( rate <= -rate_dead_zone )
                rate += rate_increment;
            else
                rate = 0.f;
        }
        // clamp steering rate
        rate = std::min( rate, max_rate );
        rate = std::max( rate, min_rate );

        // Update the command packet's speed
        dcp.setManualSpeed(speed); 

        // Update the command packet's rate of turn (steer)
        dcp.setManualSteer( rate );
    }
}
