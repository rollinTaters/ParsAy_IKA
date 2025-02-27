#include "comms_module.hpp"

CommsModule::CommsModule( Type typ ):m_type(typ)
{
#ifdef LOCALHOST
    if( m_udp.bind(54000) != sf::Socket::Status::Done )
    {
        std::cerr<<"Comms Module: failed to bind udp socket to port 54000\n";
    }
    m_selector.add(m_udp);
#endif
}

CommsModule::~CommsModule()
{
    // TODO clear memory if needed
}

bool CommsModule::sendPacket( Packet )
{
#ifdef LOCALHOST
    if( m_udp.send(
                &Packet,
                sizeof(Packet),
                sf::IpAddress::LocalHost,
                54000 )
            == sf::Socket::Status::Done )
    {
        return true;
    }
#endif
    return false;
}


bool CommsModule::sendRequest( Request )
{
    return false;
}

bool CommsModule::packetAvailable()
{
#ifdef LOCALHOST
    if( m_selector.isReady(m_udp) )
        return true;
#endif
    return false;
}

bool CommsModule::readPacket( Packet& )
{
#ifdef LOCALHOST
    std::size_t received;
    sf::IpAdress sender;
    unsigned short port;

    if( m_udp.receive(
                Packet,
                sizeof(Packet),
                received,
                sender,
                port )
            == sf::Socket::Status::Done )
    {
        return true;
    }
#endif

    return false;
}
