

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
        console_channel = 54003,    // this will be nrf24 or lora
        command_channel = 54004,
        undefined = 0
    };

    CommsModule( Type, Channel );
    ~CommsModule();

    bool openCommsChannel( Channel );

    bool sendPacket( DPacket, Channel );

    bool sendRequest( Request );

    bool packetAvailable();

    bool readPacket( DPacket& );

  private:

    Type m_type;

#ifdef LOCALHOST
    // a socket bound to classes selected channel for receiving data
    sf::UdpSocket m_receive_socket;
    // opened udp sockets for sending data
    std::vector<sf::UdpSocket*> m_vector_udp;
    sf::SocketSelector m_selector;
    // NOTE: use sf::IpAddress::LocalHost
#endif

#ifdef ARDUINO_NANO
    // TODO create m_serial, m_i2c and m_spi here
#endif

};
