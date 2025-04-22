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

#include <iostream>
#include "console_graphics.hpp"
#include "../../common_code/src/comms_module.hpp"
#include "raylib.h"

int main()
{
    std::cout << "Unmanned Land Vehicle Command Console v0.2\n";
    
    // create communications module
    CommsModule comms_module(CommsModule::udp, CommsModule::console_channel);
    // declare dummy packets
    PacketBase raw_packet;
    Drive_Telemetry_Packet1 dtp1;
    Drive_Telemetry_Packet2 dtp2;
    NGC_Telemetry_Packet ngctp;

    // graphics initialization
    const int screenWidth = 1000;
    const int screenHeight = 800;

    cg::InitWindowSafe(screenWidth,screenHeight,"Command Console");
    cg::InitObjects();
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
            comms_module.readPacket(raw_packet);

            // make sense of packet
            switch (raw_packet.packet_type)
            {
                // TODO add other type of gauges
                case drive_telemetry1:
                    dtp1 = raw_packet;
                    cg::gauge_temp->updateVal(dtp1.getMotor1Temp());
                    cg::gauge_amp->updateVal(dtp1.getMotor1Amps());
                    break;
                case drive_telemetry2:
                    dtp2 = raw_packet;
                    cg::gauge_amp2->updateVal(dtp2.getMotor2Amps());
                    cg::gauge_temp2->updateVal(dtp2.getMotor2Temp());
                    break;
                case ngc_telemetry:
                    ngctp = raw_packet;
                    cg::gauge_compass->updateVal(ngctp.getHeading());
                    cg::gauge_adi->updateRollVal(ngctp.getRoll());
                    cg::gauge_adi->updatePitchVal(ngctp.getPitch());
                    break;
                case undefined:
                    std::cerr << "Warning: Received undefined packet type" << std::endl;
                    break;
                default:
                    std::cerr << "Error: Received unknown packet type: " << raw_packet.packet_type << std::endl;
                    std::cerr << "Packet data: " << raw_packet.data1 << ", " << raw_packet.data2 << ", " << raw_packet.data3 << std::endl;
                    break;
            };
        }

        // process input
        cg::input->proccesInput();
        // render gauges
        cg::gauge_adi->render();
        cg::createPanel({550,50},{400,450},DARKGRAY,"Motor Panel");
        cg::gauge_amp2->render();
        cg::gauge_temp2->render();
        cg::gauge_amp->render();
        cg::gauge_temp->render();
        cg::gauge_speed->render();
        cg::gauge_tachometer->render();
        cg::gauge_compass->render();
        cg::gauge_battery->render();
        EndDrawing();
    }
    // a tiny cleaning
    delete cg::gauge_temp;
    delete cg::gauge_amp;
    delete cg::gauge_amp2;
    delete cg::gauge_temp2;
    delete cg::gauge_compass;
    delete cg::gauge_adi;
    delete cg::gauge_speed;
    delete cg::input;

    CloseWindow();
    std::cout << "Exiting. Have a nice day\n";
    return 0;
}
