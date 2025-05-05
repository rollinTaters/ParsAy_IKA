#pragma once
#include "comms_packets.hpp"
#include <vector>
#include <cstdint>


class VideoFeed
{
    public:
        VideoFeed(uint16_t frameWidth, uint16_t frameHight);

        //transmit data
        void setFrame( void* image_data, int image_width, int image_height );
        const std::vector<CommsPacket>& getTXPackets() const;   // packets to be transmitted
        uint16_t getCurrentFrameID() const;

        //recieve data
        void receivePacket(const CommsPacket& packet);
        bool isFrameReady() const;
        void newFrame( void* &ptr );

    private:
        // common 
        uint16_t m_width;
        uint16_t m_height;
        uint16_t m_frameID;

        //transmit
        std::vector<uint8_t> m_outgoingFrameBytes;
        std::vector<CommsPacket> m_outgoingPackets;
        void splitIntoPackets();    // splits outgoing framebytes to outgoingpackets

        //recieve
        struct FrameBuffer{
            std::vector<uint8_t> data;
            std::vector<bool> chunksRecieved;
        };

        uint16_t m_incomingFrameID;
        FrameBuffer m_incomingFrame;
};
