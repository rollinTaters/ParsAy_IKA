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

#include "comms_module.hpp"

CommsModule::CommsModule( Type typ, Channel channel ):m_type(typ)
{
#ifdef LOCALHOST
    if( m_receive_socket.bind(channel) != sf::Socket::Status::Done )
    {
        std::cerr<<"Comms Module: failed to bind udp socket to port "<<channel<<"\n";
    }
    m_selector.add(m_receive_socket);
#endif
}

CommsModule::~CommsModule()
{
#ifdef LOCALHOST
    for( sf::UdpSocket* p : m_vector_udp )
    {
        delete p;
    }
#endif
}

bool CommsModule::openCommsChannel( Channel inp )
{
#ifdef LOCALHOST
    sf::UdpSocket* new_socket = new sf::UdpSocket;
    if( new_socket->bind(inp) != sf::Socket::Status::Done )
    {
        std::cerr<<"Comms Module: failed to bind udp socket to port "<<inp<<"\n";
        return false;
    }
    m_vector_udp.push_back( new_socket );
    m_selector.add( *(m_vector_udp.back()) );
    return true;
#endif
    return false;
}

bool CommsModule::sendPacket( DPacket p, Channel c )
{
#ifdef LOCALHOST
    sf::UdpSocket *socket_to_use = nullptr;
    // find appropritate channel
    for( sf::UdpSocket* socket : m_vector_udp )
    {
        if( socket->getLocalPort() == c )
        {
            socket_to_use = socket;
        }
    }
    // if ya couldnt find the channel, create it
    if( socket_to_use == nullptr )
    {
        openCommsChannel( c );
        socket_to_use = m_vector_udp.back();
    }
    // send the packet
    if( socket_to_use->send(
                &p,
                sizeof(DPacket),
                sf::IpAddress::LocalHost,
                c )
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
    if( m_selector.isReady(m_receive_socket) )
        return true;
#endif
    return false;
}

bool CommsModule::readPacket( DPacket& p )
{
#ifdef LOCALHOST
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;

    if( m_receive_socket.receive(
                &p,
                sizeof(DPacket),
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
