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





int main()
{
    std::cout<<"Unmanned Land Vehicle Command Console v0.2\n";

    // create communications module
    CommsModule comms_module( CommsModule::udp, CommsModule::console_channel );
    // declare dummy packets
    PacketBase raw_packet;
    Drive_Telemetry_Packet1 dtp1;
    Drive_Telemetry_Packet2 dtp2;
    NGC_Telemetry_Packet ngctp;


    // graphics initialization
    sf::RenderWindow window( sf::VideoMode( 1000, 800 ), "Command Console" );
    window.setFramerateLimit(60);
    if( !init_graphics(&window) )
    {
        std::cerr<<"Failed to init graphics! Exiting.\n";
        return -1;
    }

    // main loop
    while( window.isOpen() )
    {
        // event processing
        sf::Event event;
        while( window.pollEvent(event) )
        {
            // request for closing
            if( event.type == sf::Event::Closed )
            {
                // do we wanna do any actions before closing?
                window.close();
            }
            // this handles resizing of window, shows more stuff if window is resized, (prevents stretching)
            if( event.type == sf::Event::Resized )
            {
                window.setView( sf::View( sf::FloatRect( {0,0}, sf::Vector2f(window.getSize().x,window.getSize().y) ) ) );
            }
        }   // end of event processing

        // clear window for next frame
        window.clear( sf::Color(180,180,180) );

        //do seizure inducing stuff
        DEBUG_gauge_test();

        // check incoming transmission packets
        if( comms_module.packetAvailable() )
        {
            // read packet
            comms_module.readPacket( raw_packet );

            // make sense of packet
            switch( raw_packet.packet_type )
            {
                case drive_telemetry1:
                    dtp1 = raw_packet;
                    gauge_temp.updateVal( dtp1.getMotor1Temp() );
                    gauge_amp.updateVal( dtp1.getMotor1Amps() );
                    // TODO update other gauges
                    break;
                case drive_telemetry2:

                    // TODO update engine 2's gauges
                    break;
                case ngc_telemetry:
                    
                    gauge_compass.updateVal( ngctp.getHeading() );
                    gauge_adi.updateRollVal( ngctp.getRoll() );
                    gauge_adi.updatePitchVal( ngctp.getPitch() );
                    // TODO update adi, compas
                    break;
                default:
                    // fail condition, discard packet and continue
                    break;
            };
        }
        input.proccesInput();
        // render gauges
        gauge_adi.render(window);
        gauge_amp.render(window);
        gauge_temp.render(window);
        gauge_compass.render(window);
        // a call to render stuff

        window.display();
    }


    std::cout<<"Exiting. Have a nice day\n";
}

