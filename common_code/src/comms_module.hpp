

/* 
   this is a class that manages the communications between different subsystems
   it manages the different underlying communication protocols for different systems.
   this is intended to be a global class that can be used everywhere in the codebase
*/


#include "comms_packets.hpp"

#ifdef LOCALHOST
#include "SFML/TcpSocket.hpp"
#include "SFML/UdpSocket.hpp"
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

    CommsModule( Type );
    ~CommsModule();

    bool sendPacket( Packet );

    bool sendRequest( Request );

    bool packetAvailable();

    bool readPacket( Packet& );

  private:

    Type m_type;

#ifdef LOCALHOST
    // maybe use UdpSocket instead ??
    sf::UdpSocket m_udp;
    sf::SocketSelector m_selector;
    // NOTE: use sf::IpAddress::LocalHost
#endif

#ifdef ARDUINO_NANO
    // TODO create m_serial, m_i2c and m_spi here
#endif

};
