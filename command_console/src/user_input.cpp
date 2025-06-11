/*
    MIT License

    Copyright (c) 2025 rollinTaters, guvenchemy

*/

#include "user_input.hpp"
#include <raylib.h>
//#include <iostream> // for debugging
//#include "../../common_code/src/comms_module.hpp"


UserInput::UserInput()
{
}

void UserInput::switchDriveMode( DriveMode newMode )
{
    m_drive_mode = newMode;
}

void UserInput::processInput( CommsPacket &dcp )
{
    switchDriveMode(manuel); // temporary solution
/*
    // Turret movement controls
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
                        */

    constexpr float ch_speed = 0.1f;
    if( m_drive_mode == autonomous)  // temporary solution
    {
        /*
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
        */
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

        dcp.cc_setManualSpeed(cmd_speed);
        dcp.cc_setManualSteer(cmd_rate);
    }

    // Turret Controls
    constexpr float deltaYaw = 0.01f;     
    constexpr float deltaPitch = 0.01f;    

    if (IsKeyDown(KEY_LEFT))  // ←
        dcp.cc_setTurret_azimuth(dcp.cc_getTurret_azimuth() - deltaYaw);   
    if (IsKeyDown(KEY_RIGHT)) // →
        dcp.cc_setTurret_azimuth(dcp.cc_getTurret_azimuth() + deltaYaw);    
    if (IsKeyDown(KEY_UP))    // ↑
        dcp.cc_setTurret_elevation(dcp.cc_getTurret_elevation() + deltaPitch);  
    if (IsKeyDown(KEY_DOWN))  // ↓
        dcp.cc_setTurret_elevation(dcp.cc_getTurret_elevation() - deltaPitch);  

    if( IsKeyPressed(KEY_O) )  // o
        //dcp.setTurret(1.0f);  // Toggle camera
    if( IsKeyPressed(KEY_I) ){
        dcp.cc_setLaserStatus( !dcp.cc_getLaserStatus() );//toggle laser
        
    }
/*
    // Waypoint commands
    if( IsKeyPressed( KEY_X ) )
        //dcp.ccc_setAddWaypoint(float x, float y, float z);  // Execute waypoints 
    if( IsKeyPressed( KEY_C ) )
        //dcp.getAddWaypoint(-1.0f); // Create waypoint
*/

    // Handle gamepad input
    if (IsGamepadAvailable(0)) 
    {
                
        // TODO show the state of commands issued on the screen,
        // we wanna know what kind of manual command we sent

        float leftX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float leftY = -GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y); // axis is inverted
        
        // Small joystick movements are ignored to ensure stable controls
        if (leftX > 0.1f) { // Creating deadzone because we don't want our little vehicle to shake
            dcp.cc_setManualSteer(leftX); 
        } else if (leftX < -0.1f) {
            dcp.cc_setManualSteer(leftX); 
        } else {
            dcp.cc_setManualSteer(0);
        }

        if (leftY > 0.1f) {
            dcp.cc_setManualSpeed(leftY);  
        } else if (leftY < -0.1f) {
            dcp.cc_setManualSpeed(leftY); 
        } else {
            dcp.cc_setManualSpeed(0);
        }
    }
}
