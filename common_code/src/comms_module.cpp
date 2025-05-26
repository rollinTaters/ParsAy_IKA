/*
	MIT License

	Copyright (c) 2025 rollinTaters

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

bool CommsModule::sendPacket( CommsPacket p, Channel c )
{
#ifdef LOCALHOST
    // send the packet
    if( m_udp.send(
                &p,
                sizeof(CommsPacket),
                sf::IpAddress::LocalHost,
                c )
            == sf::Socket::Status::Done )
    {
        return true;
    }
#endif
    return false;
}

bool CommsModule::sendRequest( CommsPacket::PacketType req )
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

bool CommsModule::readPacket( CommsPacket& p )
{
#ifdef LOCALHOST
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;

    if( m_udp.receive(
                &p,
                sizeof(CommsPacket),
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
