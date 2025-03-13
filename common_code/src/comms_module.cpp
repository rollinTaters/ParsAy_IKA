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
    if( m_udp.bind(channel) != sf::Socket::Status::Done )
    {
        std::cerr<<"Comms Module: failed to bind udp socket to port "<<channel<<"\n";
    }
    m_selector.add(m_udp);
#endif
}

CommsModule::~CommsModule()
{
    /*
#ifdef LOCALHOST
    for( sf::UdpSocket* p : m_vector_udp )
    {
        delete p;
    }
#endif
*/
}

bool CommsModule::sendPacket( PacketBase p, Channel c )
{
#ifdef LOCALHOST
    // send the packet
    if( m_udp.send(
                &p,
                sizeof(PacketBase),
                sf::IpAddress::LocalHost,
                c )
            == sf::Socket::Status::Done )
    {
        return true;
    }
#endif
    return false;
}


bool CommsModule::sendRequest( PacketType req )
{
    return false;
}

bool CommsModule::packetAvailable()
{
#ifdef LOCALHOST
    // NOTE: This shit may block for 10ms
    m_selector.wait( sf::seconds(0.01f) );
    if( m_selector.isReady(m_udp) )
        return true;
#endif
    return false;
}

bool CommsModule::readPacket( PacketBase& p )
{
#ifdef LOCALHOST
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;

    if( m_udp.receive(
                &p,
                sizeof(PacketBase),
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
