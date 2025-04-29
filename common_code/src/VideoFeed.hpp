#pragma once
#include <raylib.h>
#include "comms_packets.hpp"
#include <vector>
#include <cstdint>
#include <cstring> 
#include <cstdlib>
class VideoFeed
{
    public:
        VideoFeed(uint16_t frameWidth, uint16_t frameHight);

        //transmit data
        void setFrame(const Image& img);
        void splitIntoPackets();
        const std::vector<Video_Data_Packet>& getOutgoingPackets() const;
        uint16_t getCurrentFrameID() const;

        //recieve data
        void receivePacket(const Video_Data_Packet& packet);
        bool isFrameReady() const;
        Image newFrame();

    private:
        // common 
        uint16_t m_width;
        uint16_t m_height;
        uint16_t m_frameID;

        //transmit
        std::vector<uint8_t> outgoingFrameBytes;
        std::vector<Video_Data_Packet> outgoingPackets;

        //recieve
        struct FrameBuffer{
            std::vector<uint8_t> data;
            std::vector<bool> chunksRecieved;
        };

        uint16_t incomingFrameID;
        FrameBuffer incomingFrame;
};
