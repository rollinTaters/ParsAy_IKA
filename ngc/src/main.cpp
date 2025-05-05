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
#include "vehicle.hpp"
#include "env_emulator.hpp"
#include "sensor_emulator.hpp"
#include "ngc.hpp"

#ifdef DEBUG_GUI
#include "gui.hpp"  // raylib joins here
#endif

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// DEBUG -- sends env_emulators cam view to command console
#include "../../common_code/src/video_feed.hpp"
void sendCamera2Console() 
{
    static int counter = 0;
    static CommsModule undef_comms( CommsModule::udp, CommsModule::random_channel1 );
    static VideoFeed videofeed( 5 );

    std::uint8_t *raw_image_data = nullptr;
    int raw_image_data_size = 123;

    int feed_resolution_x = videofeed.getResolutionWidth();
    int feed_resolution_y = videofeed.getResolutionHeight();


    // throttling to prevent stealing all the resources
    counter++;
    if( counter > 5 )
        counter = 0;
    else
        return;

    // RenderTexture2D kamerandan görüntü al
    Image img = LoadImageFromTexture( GUI::turretViewRT.texture ); // BURADA RAM'e çekiyoruz

    ImageFormat( &img, PIXELFORMAT_UNCOMPRESSED_R8G8B8 );
    ImageResize( &img, feed_resolution_x, feed_resolution_y );

    raw_image_data = ExportImageToMemory( img, ".png", &raw_image_data_size );

    // alternative, but doesnt work
    //raw_image_data_size = (feed_resolution_x*feed_resolution_y*3);
    //std::memcpy( raw_image_data, img.data, raw_image_data_size );

    // Image processing fonksiyonuna gönder
    //ProcessImage(img);
    videofeed.TXFrame( raw_image_data, raw_image_data_size );
    std::vector<CommsPacket> tx_packets = videofeed.getTXPackets();

    /*// DEBUG        ----------------
    std::cout<<"raw image data size: "<<raw_image_data_size<<"\n";
    std::cout<<"raw image data thats being sent:\n";
    for( int i = 0; i < raw_image_data_size; i++ )
    {
        std::cout<< (int)(raw_image_data[i]) <<" ";
    }
    std::cout<<"\n";
    std::cout<<"num of tx packets: "<<tx_packets.size()<<"\n";
    std::cout<<"CommsPackets:\n";
    for( auto p : tx_packets )
    {
        std::cout<<"-->";
        for( auto d : p.data )
            std::cout<< (int)d <<" ";
        std::cout<<"<--\n";
    }
    std::cout<<"\n";
    // DEBUG END    ----------------    */

    // send 'em away
    for( CommsPacket p : tx_packets )
        if( !undef_comms.sendPacket( p, CommsModule::console_channel ) )
            std::cout<<"packet sent failed!! ";

    // RAM'den sildik (memory leak olmasın)
    UnloadImage(img);
    delete raw_image_data;
}


// this is the vehicle we are managing/controlling
Vehicle simulated_vehicle;

// our global environment emulator, if you included env_emulator.hpp, you know about its existance
Env_Emulator env_emulator( simulated_vehicle );

// our navigation guidance and control system
NGC ngc_system( &simulated_vehicle );

int main()
{
    std::cout<<"ULV NGC Emulator v0.2\n";

    // introduce ngc to the env emulator, because I gave up on a better way to do this
    env_emulator.setNGC( &ngc_system );
    env_emulator.startPhysSim();
    ngc_system.start();

#ifdef DEBUG_GUI
    SetTraceLogLevel( LOG_WARNING );    // also LOG_ERROR LOG_FATAL LOG_INFO LOG_NONE

    // this will create a window and initialize gui stuff
    GUI::initGUI();

    // after creating opengl context (initializing window) setup the model in env emulator
    env_emulator.setupModel();

    // main draw loop
    while( !WindowShouldClose() )
    {
        // this will mainly move camera around
        GUI::checkUserInput();

        BeginDrawing();
        BeginTextureMode( GUI::turretViewRT ); // --- texture mode start
        ClearBackground(SKYBLUE);

        BeginMode3D( GUI::turretCam );
        env_emulator.drawHMap();
        GUI::drawVehicle();
        EndMode3D();

        EndTextureMode();   // --------------- texture mode end

        ClearBackground( RAYWHITE );

        BeginMode3D( GUI::camera );  //--- mode 3D start

        env_emulator.drawHMap();
        GUI::drawLIDAR();
        GUI::drawAxisBillboards();
        GUI::drawVehicle();
        GUI::drawCrosshair();

        EndMode3D();  //------------- mode 3D end

        DrawFPS( 10, 10 );
        GUI::drawWPs();
        GUI::drawOverlay();

        DrawText("Turret View:", 600, 10, 10, DARKGRAY);
        DrawTextureRec( GUI::turretViewRT.texture, (Rectangle){0, 0, 200, -200}, (Vector2){600, 30}, WHITE);

        // send turret cam view to command console, debug
        sendCamera2Console();

        EndDrawing();
    }

    // this also closes the window
    env_emulator.unloadModel();
    GUI::deInitGUI();
#else   // HEADLESS MODE
    std::this_thread::sleep_for( std::chrono::seconds(10) );
#endif  // DEBUG_GUI

    std::cout<<"Exiting. Have a nice day\n";
}
