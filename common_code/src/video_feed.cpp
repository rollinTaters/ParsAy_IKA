#include "video_feed.hpp"
#include <cstdlib>
#include <cstring> 

VideoFeed::VideoFeed(uint16_t frameWidth, uint16_t frameHight){
    m_height = frameHight;
    m_width = frameWidth;
    m_frameID = 0;
    m_incomingFrameID = 0;
}


//transmit data

void VideoFeed::setFrame( void* image_data, int image_width, int image_height )
{
    m_frameID++;

    m_outgoingFrameBytes.clear();
    int bytesPerPixel = 3; //RGB
    int totalSize = image_width * image_height * bytesPerPixel;
    //resizing vector to totalSize
    m_outgoingFrameBytes.resize(totalSize);
    std::memcpy(m_outgoingFrameBytes.data(), image_data, totalSize);

    // get the packets ready for transmission
    splitIntoPackets();
}

void VideoFeed::splitIntoPackets()
{
    m_outgoingPackets.clear();
    const size_t payloadSize = 25;
    size_t totalChunks = ( m_outgoingFrameBytes.size() + payloadSize - 1 ) / payloadSize; //

    for(size_t chunkID = 0; chunkID < totalChunks; chunkID++)
    {
        CommsPacket vdp( CommsPacket::video_packet );
        vdp.setChunkID(chunkID);
        vdp.setFrameID(m_frameID);

        std::array<uint8_t, 25> payload{};

        size_t offset = chunkID * payloadSize;
        size_t copySize = std::min(payloadSize,m_outgoingFrameBytes.size() - offset);

        std::memcpy( payload.data(), &(m_outgoingFrameBytes[offset]), copySize );
        vdp.setPayload(payload);
        m_outgoingPackets.push_back(vdp);
    }
}

const std::vector<CommsPacket>& VideoFeed::getTXPackets() const
{ 
    return m_outgoingPackets; 
}


uint16_t VideoFeed::getCurrentFrameID() const
{ 
    return m_frameID; 
}


//
//recieve data
//


void VideoFeed::receivePacket(const CommsPacket& vdp)
{
    // check if given packet is indeed a video packet
    if( vdp.packet_type != CommsPacket::video_packet ) return;

    //get packet
    uint16_t packetFrameID = vdp.getFrameID();
    uint16_t chunkID = vdp.getChunkID();
    //if there is new frames then remove the old one
    if (packetFrameID != m_incomingFrameID)
    {
        m_incomingFrameID = packetFrameID;
        m_incomingFrame.data.clear();
        m_incomingFrame.chunksRecieved.clear();

        int bytesPerPixel = 3;
        size_t totalSize = m_width * m_height * bytesPerPixel;

        m_incomingFrame.data.resize(totalSize, 0);
        m_incomingFrame.chunksRecieved.resize(totalSize + 24 / 25, false);

    }
    size_t offset = chunkID * 25;
    auto payload = vdp.getPayload();

    size_t copySize = std::min<size_t>(25, m_incomingFrame.data.size() - offset);
    std::memcpy(m_incomingFrame.data.data() + offset, payload.data(), copySize);

    if(chunkID < m_incomingFrame.chunksRecieved.size()){
        m_incomingFrame.chunksRecieved[chunkID] = true;
    }

    
}

bool VideoFeed::isFrameReady() const
{
    for (bool recieved : m_incomingFrame.chunksRecieved){
        if (!recieved){
            return false;
        }
    }
    return true;
    // check chunks 
}

void VideoFeed::newFrame( void* &ptr )
{
    // WARNING: following code plays with fire.

    //reconstruct the Frame 
    /*
    Image reFrame = {
        .width = m_width,
        .height = m_height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
    };
    */

    // because memory leaks are bad
    std::free( ptr );
    ptr = malloc(m_incomingFrame.data.size());
    std::memcpy(ptr, m_incomingFrame.data.data(), m_incomingFrame.data.size());
}
