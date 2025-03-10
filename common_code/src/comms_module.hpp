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
   this is a class that manages the communications between different subsystems
   it manages the different underlying communication protocols for different systems.
   this is intended to be a global class that can be used everywhere in the codebase
*/

// FORCE DEFINE LOCALHOST FOR TESTING
#define LOCALHOST

#include "comms_packets.hpp"
#include <iostream> // cerr

#ifdef LOCALHOST
//#include "SFML/TcpSocket.hpp"
#include "SFML/Network.hpp"
#endif

#ifdef ARDUINO_NANO
#include "SPI.hpp"
#define MISO_PIN xx
#define MOSI_PIN xx
#define CS_PIN xx
#endif

class CommsModule
{
  public:
    enum Type{
        udp,
        tcp,
        spi,
        i2c
    };

    enum Channel{
        ngc_channel = 54000,
        drive_channel = 54001,
        turret_channel = 54002,
        console_channel = 54003,    // this will later be linked to nrf24 or lora
        command_channel = 54004,
        random_channel1 = 54005,
        random_channel2 = 54006,
        undefined = 0
    };

    CommsModule( Type, Channel );
    ~CommsModule();

    bool sendPacket( DPacket, Channel );

    bool sendRequest( Request );

    bool packetAvailable();

    bool readPacket( DPacket& );

  private:

    Type m_type;

#ifdef LOCALHOST
    // a socket bound to classes selected channel for rx/tx of data
    sf::UdpSocket m_udp;
    sf::SocketSelector m_selector;
    // NOTE: use sf::IpAddress::LocalHost
#endif

#ifdef ARDUINO_NANO
    // TODO create m_serial, m_i2c and m_spi here
#endif

};
