#include "VideoFeed.hpp"

VideoFeed::VideoFeed(uint16_t frameWidth, uint16_t frameHight){
    m_height = frameHight;
    m_width = frameWidth;
    m_frameID = 0;
    incomingFrameID = 0;
}


//transmit data

void VideoFeed::setFrame(const Image& img)
{
    outgoingFrameBytes.clear();
    int bytesPerPixel = 3; //RGB
    int totalSize = img.width * img.height * bytesPerPixel;
    //resizing vector to totalSize
    outgoingFrameBytes.resize(totalSize);
    std::memcpy(outgoingFrameBytes.data(),img.data, totalSize);
    
    m_frameID++;
}

void VideoFeed::splitIntoPackets()
{
    outgoingPackets.clear();
    const size_t payloadSize = 25;
    size_t totalChunks = ( outgoingFrameBytes.size() + payloadSize - 1 ) / payloadSize; //

    for(size_t chunkID = 0; chunkID < totalChunks; chunkID++)
    {
        Video_Data_Packet vdp;
        vdp.setChunkID(chunkID);
        vdp.setFrameID(m_frameID);

        std::array<uint8_t, 25> payload{};

        size_t offset = chunkID * payloadSize;
        size_t copySize = std::min(payloadSize,outgoingFrameBytes.size() - offset);

        std::memcpy(payload.data(), incomingFrame.data.data(), payloadSize);
        vdp.setPayload(payload);
        outgoingPackets.push_back(vdp);
    }
}

const std::vector<Video_Data_Packet>& VideoFeed::getOutgoingPackets() const
{ 
    return outgoingPackets; 
}


uint16_t VideoFeed::getCurrentFrameID() const
{ 
    return m_frameID; 
}


//
//recieve data
//


void VideoFeed::receivePacket(const Video_Data_Packet& vdp)
{
    //get packet
    uint16_t packetFrameID = vdp.getFrameID();
    uint16_t chunkID = vdp.getChunkID();
    //if there is new frames then remove the old one
    if (packetFrameID != incomingFrameID)
    {
        incomingFrameID = packetFrameID;
        incomingFrame.data.clear();
        incomingFrame.chunksRecieved.clear();

        int bytesPerPixel = 3;
        size_t totalSize = m_width * m_height * bytesPerPixel;

        incomingFrame.data.resize(totalSize, 0);
        incomingFrame.chunksRecieved.resize(totalSize + 24 / 25, false);

    }
    size_t offset = chunkID * 25;
    auto& payload = vdp.getPayload();

    size_t copySize = std::min<size_t>(25, incomingFrame.data.size() - offset);
    std::memcpy(incomingFrame.data.data() + offset, payload.data(), copySize);

    if(chunkID < incomingFrame.chunksRecieved.size()){
        incomingFrame.chunksRecieved[chunkID] = true;
    }

    
}

bool VideoFeed::isFrameReady() const
{
    for (bool recieved : incomingFrame.chunksRecieved){
        if (!recieved){
            return false;
        }
    }
    return true;
    // check chunks 
}

Image VideoFeed::newFrame()
{
    //reconstruct the Frame 
    Image reFrame = {
        .width = m_width,
        .height = m_height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
    };
    reFrame.data = malloc(incomingFrame.data.size());
    std::memcpy(reFrame.data, incomingFrame.data.data(), incomingFrame.data.size());

    return reFrame;
}