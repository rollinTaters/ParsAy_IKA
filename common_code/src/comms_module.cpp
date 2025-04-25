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

int CommsModule_pipe::checkFifoPipe( PipeName pipe_name )
{
    struct stat statStruct;
    int result;
    result = stat( pipe_files[pipe_name], &statStruct );
    return result;
}

int CommsModule_pipe::createFifoPipe( PipeName pipe_name )
{
    int result;
    result = mkfifo( pipe_files[pipe_name], 0666 );
    return result;
}

CommsModule_pipe::CommsModule_pipe( PipeName rx, PipeName tx )
{
    // setup tx&rx channels accordingly
    m_tx_pipe = tx;
    m_rx_pipe = rx;

    PipeName pipes[] = { m_tx_pipe, m_rx_pipe };

    // check if we have a file created for pipe and its ready
    for( PipeName pipe_name : pipes )
    {
        if( checkFifoPipe( pipe_name ) == -1 )
        {   // create pipe
            std::cout<<"creating pipe\n";   // DEBUG
            if( createFifoPipe( pipe_name ) == -1 )
                std::cerr<<"Comms Module: Failed to create Fifo pipe\n";
            // TODO: if we failed.. what then??
        }
    }
}

bool CommsModule_pipe::sendPacket( PacketBase pb )
{
    return sendPacket( pb, m_tx_pipe );
}

bool CommsModule_pipe::sendPacket( PacketBase pb, PipeName tx )
{
    if( tx != m_tx_pipe )
    {
        // TODO
        // make sure we have a pipe setup for that
    }

    m_pipe.open( pipe_files[tx], std::fstream::out );
    m_pipe << pb << std::endl; // write packet to file
    m_pipe.flush();  // make sure kernel writes the changes to the file

    // TODO does this block?? for how long??

    m_pipe.close();  // so that the reader can open the pipe and read
    return true;
}

bool CommsModule_pipe::readPacket( PacketBase& pb )
{
    // TODO max tries? timeout ?
    if( checkFifoPipe( m_rx_pipe ) != 0 )
    {
        // failed to connect to pipe
        // TODO wait a bit..
    }

    m_pipe.open( pipe_files[m_rx_pipe], std::ios::in );
    getline( m_pipe, pb );
    m_pipe.close();  // so that it is ready for write
    return true;
}

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
