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

#ifdef DEBUG_GUI
#include "raylib.h"
#endif

// this is the vehicle we are managing/controlling
Vehicle simulated_vehicle;

// our global environment emulator, if you included env_emulator.hpp, you know about its existance
Env_Emulator env_emulator( simulated_vehicle );

int main()
{
    std::cout<<"ULV NGC Emulator v0.2\n";

    // our navigation guidance and control system
    NGC ngc_system( &simulated_vehicle );

    env_emulator.startPhysSim();
    ngc_system.start();

#ifdef DEBUG_GUI
    // create a window for gui rendering
    InitWindow( 1000,800, "NGC DEBUG GUI" );

    // camera setup
    Camera camera = {0};
    camera.position = (Vector3){ 0.f, 10.f, 10.f };
    camera.target = (Vector3){ 0.f, 0.f, 0.f };
    camera.up = (Vector3){ 0.f, 1.f, 0.f };
    camera.fovy = 45.f;
    camera.projection = CAMERA_PERSPECTİVE;

    SetTargetFPS(60);

    // main draw loop
    while( !WindowShouldClose() )
    {
        BeginDrawing();
        ClearBackground( RAYWHITE );

        BeginMode3D( camera );  //--- mode 3D start

        // position, radius_top, radius_bottom, height, sides, color
        DrawCylinder( (Vector3){2,0,0}, 2, 2, 3, 5, SKYBLUE );

        DrawFPS( 10, 10 );
        EndMode3D();  //------------- mode 3D end

        EndDrawing();
    }

    // de-initialization of window and opengl context
    CloseWindow();

#endif  // DEBUG_GUI

    //std::this_thread::sleep_for( std::chrono::seconds(10) );

    std::cout<<"Exiting. Have a nice day\n";
}
