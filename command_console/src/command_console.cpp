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


int main()
{
    std::cout << "Unmanned Land Vehicle Command Console v0.2\n";
    
    // create communications module
    CommsModule comms_module(CommsModule::udp, CommsModule::console_channel);
    VideoFeed streamer( 1 );    // using resolution mode 1

    // declare dummy packets
    CommsPacket packet; // this one we use for the data we received
    CommsPacket packet2send( CommsPacket::console_command ); // this one we use for sending console packets

    Image video_frame;
    std::uint8_t *video_frame_raw;
    size_t video_frame_size;
    Texture2D video_texFrame;

    // graphics initialization
    const int screenWidth = 1900;
    const int screenHeight = 900;

    cg::InitWindowSafe(screenWidth,screenHeight,"Command Console");


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
        if(IsKeyDown(KEY_T)){

            cg::DEBUG_gauge_test();
        }
        
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
                    std::cout<<"received video packet. frameID: "<<streamer.getCurrentFrameID()<<"\n";
                    if( streamer.isFrameReady() )
                    {
                        std::cout<<"frame is ready and will be rendered!!\n";
                        streamer.RXFrame( video_frame_raw, video_frame_size );
                        video_frame = LoadImageFromMemory( "jpg", video_frame_raw, video_frame_size );
                        // NOTE: loading an image (in RAM) to a texture (in VRAM) is expensive
                        video_texFrame = LoadTextureFromImage(video_frame);
                        //frame, pos x, pos y, tint
                        DrawTexture(video_texFrame, 960,50, WHITE);
                        UnloadImage(video_frame);
                        UnloadTexture(video_texFrame);
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
        EndDrawing();

        // process input
        cg::input.processInput( packet2send );
        comms_module.sendPacket( packet2send, CommsModule::command_channel );
    }

    CloseWindow();
    std::cout << "Exiting. Have a nice day\n";
    return 0;
}


