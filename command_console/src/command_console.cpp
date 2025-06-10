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

#include <iostream>
#include "console_graphics.hpp"
#include "../../common_code/src/comms_module.hpp"
#include "../../common_code/src/video_feed.hpp"
#include "raylib.h"
#include <chrono>
#include <thread>

int main()
{
    std::cout << "Unmanned Land Vehicle Command Console v0.2\n";
    
    // create communications module
    CommsModule comms_module(CommsModule::udp, CommsModule::console_channel);
    VideoFeed streamer( 5 );    // using resolution mode 1

    // declare dummy packets
    CommsPacket packet; // this one we use for the data we received
    CommsPacket packet2send( CommsPacket::console_command ); // this one we use for sending console packets

    // graphics initialization
    const int screenWidth = 1900;
    const int screenHeight = 900;

    cg::InitWindowSafe(screenWidth,screenHeight,"Command Console");

    Image video_frame;
    ImageFormat( &video_frame, PIXELFORMAT_UNCOMPRESSED_R8G8B8 );
    video_frame.width = streamer.getResolutionWidth();
    video_frame.height = streamer.getResolutionHeight();
    video_frame.mipmaps = 1;

    std::uint8_t *video_frame_raw = new std::uint8_t[1024]{0};
    size_t video_frame_size;
    Texture2D video_texFrame;
    

    // main loop
    while (!WindowShouldClose())
    {
        // event processing
        if (IsKeyPressed(KEY_ESCAPE)) {
            // ESC to close the application
            break;
        }            
        // clear window for next frame
        BeginDrawing();
        ClearBackground(Color{180, 180, 180, 255});
        // press t for debug test
        if(IsKeyDown(KEY_SPACE)){

            cg::DEBUG_gauge_test();
        }
        DrawText(TextFormat("FPS: %d", GetFPS()), 1600, 10, 20, BLACK);  
        
        // check incoming transmission packets
        if (comms_module.packetAvailable())
        {
            // read packet
            comms_module.readPacket(packet);

            // make sense of packet
            switch (packet.packet_type)
            {
                case CommsPacket::console_telemetry:
                    cg::gauge_temp.updateVal(packet.ct_getMotor1Temp());
                    cg::gauge_amp.updateVal (packet.ct_getMotor1Amps());
                    cg::gauge_amp2.updateVal(packet.ct_getMotor2Amps());
                    cg::gauge_temp2.updateVal   (packet.ct_getMotor2Temp());
                    cg::gauge_compass.updateVal (packet.ct_getHeading());
                    cg::gauge_adi.updateRollVal (packet.ct_getRoll());
                    cg::gauge_adi.updatePitchVal(packet.ct_getPitch());
                    cg::gauge_speed.updateVal   (packet.ct_getSpeed());
                    break;

                case CommsPacket::console_command:
                    std::cerr<<" why are we receiving a console command packet when we should be the one sending it?\n";
                    break;

                case CommsPacket::video_packet:
                    streamer.receivePacket( packet );
                    /*// DEBUG        ----------------
                    std::cout<<"received video packet. frameID: "<<streamer.getCurrentFrameID()<<"\n";
                    for( auto d : packet.data )
                    {
                        std::cout<< (int)d <<" ";
                    }
                    std::cout<<"\n";
                    // DEBUG END    ---------------- */

                    if( streamer.isFrameReady() )
                    {
                        streamer.RXFrame( video_frame_raw, video_frame_size );
                        /*// DEBUG        ----------------
                        std::cout<<"Command Console Main.cpp:\nRX frame size: "
                            <<video_frame_size<<"\n";
                        std::cout<<"raw frames:\n";
                        for( int i = 0; i < video_frame_size; i++ )
                        {
                            std::cout<< (int)( video_frame_raw[i] ) << " ";
                        }
                        std::cout<<"\n";
                        // DEBUG END    ---------------- */

                        //ImageFormat( &video_frame, PIXELFORMAT_UNCOMPRESSED_R8G8B8 );  // revert back to 3 channels
                        video_frame = LoadImageFromMemory( ".png", video_frame_raw, video_frame_size );

                        // because there is a bug in raylib and UpdateTexture function only accepts this format
                        //ImageFormat( &video_frame, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 ); 
                        //Color* pixels = LoadImageColors( video_frame );
                        //UpdateTexture( video_texFrame, pixels );

                        video_texFrame = LoadTextureFromImage(video_frame);
                        UnloadImage(video_frame);
                    }
                    break;
                
                case CommsPacket::undefined:
                    std::cerr << "Warning: Received undefined packet type" << std::endl;
                    break;

                default:
                    std::cerr << "Error: Received unknown packet type: " << (int)packet.packet_type << std::endl;
                    break;
            }
        }

        // render gauges
        cg::renderGauges();
        DrawTextureEx( video_texFrame, {960,50}, 0.f, 16.f, WHITE);
        EndDrawing();

        // process input
        cg::input.processInput( packet2send );

        // DEBUG
        std::cout<<"we be sending this data: \n";
        for( auto d: packet2send.data )
        {
            std::cout<<(int)d<<" ";
        }
        std::cout<<"\n";
        // DEBUG END

         // for testing purposes we send it to ngc, normally we wanna send to ccm
        comms_module.sendPacket( packet2send, CommsModule::ngc_channel );
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // this is why our command console is running at 5 fps but idk how to fix it.
    }


    UnloadTexture(video_texFrame);
    CloseWindow();
    std::cout << "Exiting. Have a nice day\n";
    return 0;
}


