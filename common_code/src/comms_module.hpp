/*
	MIT License

	Copyright (c) 2025 rollinTaters

   this is a class that manages the communications between different subsystems
   it manages the different underlying communication protocols for different systems.
   this is intended to be a global class that can be used everywhere in the codebase
*/

// FORCE DEFINE LOCALHOST FOR TESTING
#pragma once
#define LOCALHOST

#include "comms_packets.hpp"
#include <iostream> // cerr

#ifdef LOCALHOST
//#include "SFML/TcpSocket.hpp"
#include "SFML/Network.hpp"
#endif

#include <sys/stat.h>   // checking status of pipe files
#include <unistd.h>
#include <fstream>  // read/write pipe files


#ifdef ARDUINO_NANO
#include "SPI.hpp"
#define MISO_PIN xx
#define MOSI_PIN xx
#define CS_PIN xx
#endif

class CommsModule_pipe
{
  public:
    enum PipeName
    {
        pipe_ngc,
        pipe_ip,
        pipe_turret
    };


    CommsModule_pipe( PipeName rx, PipeName tx );
    ~CommsModule_pipe();

    bool sendPacket( CommsPacket );
    bool sendPacket( CommsPacket, PipeName );
    bool readPacket( CommsPacket& );
    // TODO packet available, maybe use last modified time?
    bool packetAvailable(); // always returns true for now

  private:
    int checkFifoPipe( PipeName );
    int createFifoPipe( PipeName );

    PipeName m_rx_pipe;
    PipeName m_tx_pipe;

    std::fstream m_pipe;

    static constexpr char* pipe_files[] = {
        "/tmp/pipe0",
        "/tmp/pipe1",
        "/tmp/pipe2"
    };


};

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
        ccm_channel = 54004,        // this will also
        random_channel1 = 54005,
        random_channel2 = 54006,
        undefined = 0
    };

    CommsModule( Type, Channel );
    ~CommsModule();

    bool sendPacket( CommsPacket, Channel );

    bool sendRequest( CommsPacket::PacketType );

    bool packetAvailable();

    bool readPacket( CommsPacket& );

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


