/*
	MIT License

	Copyright (c) 2025 rollinTaters, guvenchemy

*/

//#include <iostream>
#include <chrono>   // throttling of packet sends
#include <thread>   // this_thread::sleep_for
#include "console_graphics.hpp"
#include "../../common_code/src/comms_module.hpp"
#include "../../common_code/src/video_feed.hpp"
#include "raylib.h"
#include <chrono>
#include <thread>
#include "gui.hpp"

// Global variable definitions for GUI
Vehicle simulated_vehicle;
Env_Emulator env_emulator(simulated_vehicle);
NGC ngc_system(&simulated_vehicle);

int main()
{
    std::cout << "Unmanned Land Vehicle Command Console v0.2\n";
    

    // create communications module
    CommsModule comms_module(CommsModule::udp, CommsModule::console_channel);
    VideoFeed streamer( 3 );    // using resolution mode 3
    std::chrono::steady_clock m_clock;
    std::chrono::milliseconds m_frame_cent_interval = std::chrono::milliseconds(50);
    std::chrono::time_point<std::chrono::steady_clock> m_frame_cent_time;

    // declare dummy packets
    CommsPacket packet; // this one we use for the data we received
    CommsPacket packet2send( CommsPacket::console_command ); // this one we use for sending console packets


    // graphics initialization
    const int screenWidth = 1900;
    const int screenHeight = 900;

    // Initialize gauges (but don't create window - that's done by GUI::initGUI)
    cg::InitWindowSafe(screenWidth,screenHeight,"Command Console");

    // Initialize GUI for 3D visualization
    GUI::initGUI();
    
    // Setup environment emulator model after OpenGL context is created
    env_emulator.setupModel();

    Image video_frame = GenImageColor(
                            streamer.getResolutionWidth(),
                            streamer.getResolutionHeight(),
                            DARKGRAY );
    ImageDrawText( &video_frame, "No Signal", video_frame.width/3.f, video_frame.height/2.f, 20, RED );
    video_frame.mipmaps = 1;
    ImageFormat( &video_frame, PIXELFORMAT_UNCOMPRESSED_R8G8B8 );

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

        // 3D Visualization (left half only)
        BeginMode3D(GUI::camera);
        
        // Draw 3D environment based on real sensor data
        GUI::drawLIDAR();        // LIDAR points from real sensors
        GUI::drawMapPoints();    // SLAM points from real sensors
        GUI::drawVehicleBody();  // Vehicle body in 3D
        GUI::drawTurret();       // Turret in 3D
        GUI::drawWPs();          // Waypoints in 3D
        GUI::drawPredictOPs();   // Predicted trajectory
        GUI::drawAxisBillboards(); // Coordinate system
        
        EndMode3D();

        // 2D Elements (right side of screen)
        // Video feed
        DrawTextureEx( video_texFrame, {960,50}, 0.f, 3.f, WHITE);
        if(IsKeyDown(KEY_SPACE)){
            cg::DEBUG_gauge_test();
        }
        // FPS 
        DrawText(TextFormat("FPS: %d", GetFPS()), 1600, 10, 20, BLACK);
        
        // GUI overlay with vehicle info
        GUI::drawOverlay();

        // check incoming transmission packets, ALL OF THEM.
        while (comms_module.packetAvailable())
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
                    cg::steering_wheel.updateVal(packet.cc_getManualSteer());
                    
                    // Update simulated vehicle with real telemetry data for 3D visualization
                    // This will make the 3D vehicle position match the real vehicle
                    // Note: We'll need to add methods to update vehicle position/attitude
                    break;

                case CommsPacket::console_command:
                    std::cerr<<" why are we receiving a console command packet when we should be the one sending it?\n";
                    break;

                case CommsPacket::video_packet:
                    streamer.receivePacket( packet );
                    if( streamer.isFrameReady() )
                    {
                        streamer.RXFrame( video_frame_raw, video_frame_size );
                        UnloadImage(video_frame);
                        video_frame = LoadImageFromMemory( ".png", video_frame_raw, video_frame_size );
                        UnloadTexture( video_texFrame );
                        video_texFrame = LoadTextureFromImage(video_frame);
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

        std::cout<<"we be sending this data: \n";
        for( auto d: packet2send.data )
        {
            std::cout<<(int)d<<" ";
        }
        std::cout<<"\n";


        // check if its time to send packet
        if( m_clock.now() < m_frame_cent_time + m_frame_cent_interval )
        {
            std::this_thread::sleep_for( m_frame_cent_interval /5 );
            continue;
        }

        comms_module.sendPacket( packet2send, CommsModule::ngc_channel );
        m_frame_cent_time = m_clock.now();
    }



    UnloadTexture(video_texFrame);
    UnloadImage(video_frame);
    
    // Cleanup GUI and environment emulator
    env_emulator.unloadModel();
    GUI::deInitGUI();
    
    CloseWindow();
    std::cout << "Exiting. Have a nice day\n";
    return 0;
}


