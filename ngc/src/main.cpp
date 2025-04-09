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
#include "gui.hpp"  // raylib joins here
#endif


// this is the vehicle we are managing/controlling
Vehicle simulated_vehicle;

// our global environment emulator, if you included env_emulator.hpp, you know about its existance
Env_Emulator env_emulator( simulated_vehicle );

// our navigation guidance and control system
NGC ngc_system( &simulated_vehicle );

int main()
{
    std::cout<<"ULV NGC Emulator v0.2\n";


    env_emulator.startPhysSim();
    ngc_system.start();

#ifdef DEBUG_GUI
    // this will create a window and initialize gui stuff
    GUI::initGUI();

    // main draw loop
    while( !WindowShouldClose() )
    {
        // this will mainly move camera around
        GUI::checkUserInput();

        BeginDrawing();
        ClearBackground( RAYWHITE );

        BeginMode3D( GUI::camera );  //--- mode 3D start

        GUI::drawAxisBillboards();
        GUI::drawVehicle();
        GUI::drawCrosshair();

        EndMode3D();  //------------- mode 3D end

        DrawFPS( 10, 10 );
        GUI::drawWPs();
        GUI::drawOverlay();

        EndDrawing();
    }

    // this also closes the window
    GUI::deInitGUI();
#else   // HEADLESS MODE
    std::this_thread::sleep_for( std::chrono::seconds(10) );
#endif  // DEBUG_GUI

    std::cout<<"Exiting. Have a nice day\n";
}
