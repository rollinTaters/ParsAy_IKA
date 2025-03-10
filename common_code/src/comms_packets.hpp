

/*
   These are different packets that we use to transfer data between different subsystems
   Feel free to add your own packets if needed
*/
#include <cstdint>

enum Request
{
    request1,
    request2,
    request3
};

// use sf::Packet ???
struct DPacket
{
    std::int16_t data1;
    std::int16_t data2;
};
