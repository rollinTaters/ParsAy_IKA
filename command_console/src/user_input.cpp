/*
    MIT License

    Copyright (c) 2025 rollinTaters, guvenchemy

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
#include "user_input.hpp"
#include <raylib.h>
#include <iostream>
#include "../../common_code/src/comms_module.hpp"


UserInput::UserInput()
{
   
}
void UserInput::switchDriveMode(DriveMode newMode){
    m_drive_mode = newMode;
}

void UserInput::processInput( CommsPacket &dcp ){
    dcp.packet_type = CommsPacket::ngc_command;


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

    // Camera movement controls
    if( IsKeyDown( KEY_H ) )
        dcp.setCamera(-2*DEG2RAD);  // Rotate camera left
    if( IsKeyDown( KEY_L ) )
        dcp.setCamera(2*DEG2RAD);   // Rotate camera right
    if( IsKeyDown( KEY_J ) )
        dcp.setCamera(-2*DEG2RAD);  // Rotate camera down
    if( IsKeyDown( KEY_K ) )
        dcp.setCamera(2*DEG2RAD);   // Rotate camera up
    if( IsKeyDown( KEY_N ) )
        dcp.setCamera(0.2f);        // Zoom out
    if( IsKeyDown( KEY_M ) )
        dcp.setCamera(-0.2f);       // Zoom in

    // Hat control mode toggle
    if( IsKeyPressed( KEY_T ) )
        if( dcp.getHatMode() > 0 )
            dcp.setHatMode(-1.0f);  // Toggle hat mode
        else
            dcp.setHatMode(1.0f);  // Toggle hat mode

    constexpr float ch_speed = 0.1f;
    if( dcp.getHatMode() > 0 )  // Hat mode is active
    {
        // Moving crosshair
        if( IsKeyDown( KEY_W ) )
            dcp.setCrosshair(ch_speed);  
        if( IsKeyDown( KEY_S ) )
            dcp.setCrosshair(-ch_speed);  
        if( IsKeyDown( KEY_A ) )
            dcp.setCrosshair(-ch_speed);  
        if( IsKeyDown( KEY_D ) )
            dcp.setCrosshair(ch_speed);   
        if( IsKeyDown( KEY_R ) )
            dcp.setCrosshair(ch_speed);   
        if( IsKeyDown( KEY_F ) )
            dcp.setCrosshair(-ch_speed);  
    }
    else  // Manual control mode
    {
        float cmd_speed = 0;
        float cmd_rate = 0;
        const float cmd_nom_speed = 1.2f;
        const float cmd_nom_rate = 10 * (PI/180.f);

        if( IsKeyDown( KEY_W ) )
            cmd_speed = cmd_nom_speed;
        if( IsKeyDown( KEY_S ) )
            cmd_speed = -1*cmd_nom_speed;
        if( IsKeyDown( KEY_A ) )
            cmd_rate = cmd_nom_rate;
        if( IsKeyDown( KEY_D ) )
            cmd_rate = -1*cmd_nom_rate;

        dcp.setManualSpeed(cmd_speed);
        dcp.setManualSteer(cmd_rate);
    }

    // Turret controls
    const float deltaYaw = 0.01f;
    const float deltaPitch = 0.01f;

    if (IsKeyDown(KEY_LEFT))  // ←
        dcp.setTurret(deltaYaw);    // Rotate left
    if (IsKeyDown(KEY_RIGHT)) // →
        dcp.setTurret(-deltaYaw);   // Rotate right
    if (IsKeyDown(KEY_UP))    // ↑
        dcp.setTurret(deltaPitch);  // Rotate up
    if (IsKeyDown(KEY_DOWN))  // ↓
        dcp.setTurret(-deltaPitch); // Rotate down

    if( IsKeyPressed(KEY_O) )  // o
        dcp.setTurret(1.0f);  // Toggle camera
    if( IsKeyPressed(KEY_I) )  // i
        dcp.setTurret(-1.0f); // Toggle laser

    // Waypoint commands
    if( IsKeyPressed( KEY_X ) )
        dcp.setWaypoint(1.0f);  // Execute waypoints
    if( IsKeyPressed( KEY_C ) )
        dcp.setWaypoint(-1.0f); // Create waypoint

    // Handle gamepad input
    if (IsGamepadAvailable(0)) 
    {
                
        // TODO show the state of commands issued on the screen,
        // we wanna know what kind of manual command we sent

        float leftX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float leftY = -GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y); // axis is inverted
        
        // Small joystick movements are ignored to ensure stable controls
        if (leftX > 0.1f) { // Creating deadzone because we don't want our little vehicle to shake
            dcp.setManualSteer(leftX); 
        } else if (leftX < -0.1f) {
            dcp.setManualSteer(leftX); 
        } else {
            dcp.setManualSteer(0);
        }

        if (leftY > 0.1f) {
            dcp.setManualSpeed(leftY);  
        } else if (leftY < -0.1f) {
            dcp.setManualSpeed(leftY); 
        } else {
            dcp.setManualSpeed(0);
        }
    }
}
