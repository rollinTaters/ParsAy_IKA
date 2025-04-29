#pragma once
#include <cstdint>
#include <array>
// Base for all packet types, this is the data that is actually being sent
// other packet types only employ custom methods to read/decode this data
struct PacketBase
{
    std::uint8_t packet_type;
    std::int16_t data1;
    std::int16_t data2;
    std::int16_t data3;
};

enum PacketType{
    undefined,
    drive_command,
    drive_telemetry1,
    drive_telemetry2,
    turret,
    ngc_command,
    ngc_telemetry,
    video_data,

    request1,
    request2,
    request3
};

/*
   These are different packets that we use to transfer data between different subsystems
   Feel free to add your own packets if needed
*/

// data sent to the drive module
struct Drive_Command_Packet : public PacketBase
{
    Drive_Command_Packet(){ packet_type = drive_command; data1 = 0; data2 = 0;}
    Drive_Command_Packet( PacketBase& pb )
    {
        packet_type = pb.packet_type;
        data1 = pb.data1;
        data2 = pb.data2;
    }
    std::uint16_t getSpeed(){ return data1; }
    std::uint16_t getSteer(){ return data2; }

    void setSpeed( std::uint16_t inp ){ data1 = inp; }
    void setSteer( std::uint16_t inp ){ data2 = inp; }
};

// data sent from drive module
struct Drive_Telemetry_Packet1 : public PacketBase
{
    Drive_Telemetry_Packet1(){ packet_type = drive_telemetry1; data1 = 0; data2 = 0; data3 = 0; }
    Drive_Telemetry_Packet1( PacketBase& pb )
    {
        packet_type = pb.packet_type;
        data1 = pb.data1;
        data2 = pb.data2;
        data3 = pb.data3;
    }

    std::uint16_t getMotor1Temp(){ return data1; }
    std::uint16_t getMotor1Amps(){ return data2; }
    std::uint16_t getMotor1Vel() { return data3; }

    void setMotor1Temp( std::uint16_t inp ){ data1 = inp; }
    void setMotor1Amps( std::uint16_t inp ){ data2 = inp; }
    void setMotor1Vel ( std::uint16_t inp ){ data3 = inp; }
};
struct Drive_Telemetry_Packet2 : public PacketBase
{
    Drive_Telemetry_Packet2(){ packet_type = drive_telemetry2; data1 = 0; data2 = 0; data3 = 0; }
    Drive_Telemetry_Packet2( PacketBase& pb )
    {
        packet_type = pb.packet_type;
        data1 = pb.data1;
        data2 = pb.data2;
        data3 = pb.data3;
    }

    std::uint16_t getMotor2Temp(){ return data1; }
    std::uint16_t getMotor2Amps(){ return data2; }
    std::uint16_t getMotor2Vel() { return data3; }

    void setMotor1Temp( std::uint16_t inp ){ data1 = inp; }
    void setMotor2Amps( std::uint16_t inp ){ data2 = inp; }
    void setMotor3Vel ( std::uint16_t inp ){ data3 = inp; }
};


// data sent/received to/from the turret module
struct Turret_Packet : public PacketBase
{
    std::uint16_t getPan() { return data1; }
    std::uint16_t getTilt(){ return data2; }
    std::uint16_t getLaserStatus(){ return data3; }

    void setPan ( std::uint16_t inp ){ data1 = inp; }
    void setTilt( std::uint16_t inp ){ data2 = inp; }
    void setLaserStatus( std::uint16_t inp ){ data3 = inp; }
};


// data sent to the ngc module
struct NGC_Command_Packet : public PacketBase
{
    std::uint16_t getControlMode(){ return data1; }

    void setControlMode( std::uint16_t inp ){ data1 = inp; }
};

// from NGC to CCM
struct NGC_Telemetry_Packet : public PacketBase
{
    NGC_Telemetry_Packet(){ packet_type = ngc_telemetry; data1 = 0; data2 = 0; data3 = 0; }
    NGC_Telemetry_Packet( PacketBase& pb )
    {
        packet_type = pb.packet_type;
        data1 = pb.data1;
        data2 = pb.data2;
        data3 = pb.data3;
    }
    std::uint16_t getHeading(){ return data1; }
    std::uint16_t getPitch(){ return data2; }
    std::uint16_t getRoll(){ return data3; }

    void setHeading( std::uint16_t inp ){ data1 = inp; }
    void setPitch  ( std::uint16_t inp ){ data2 = inp; }
    void setRoll   ( std::uint16_t inp ){ data3 = inp; }
};

struct Video_Data_Packet : public PacketBase
{
    std::array<std::uint8_t, 25> payload;
    Video_Data_Packet(){ packet_type = video_data;  data1 = 0; data2 = 0; data3 = 0; payload.fill(0); }
    Video_Data_Packet( PacketBase& pb )
    {
        packet_type = pb.packet_type;
        data1 = pb.data1;        // frame id
        data2 = pb.data2 & 0xFF; // chunk id ( check out what bitwise and bit masking is )
        // data3 = (pb.data2 >> 8) & 0xFF; // total chunks ( optional )
        payload.fill(0); 
    }
    std::uint16_t getFrameID() const { return data1;}
    std::uint16_t getChunkID() const { return data2;}
    //std::uint16_t getTotalChunks() const { return data3;} // optional
    const std::array<std::uint8_t, 25>& getPayload() const { return payload; } 

    void setFrameID(std::uint16_t id) { data1 = id; }
    void setChunkID(std::uint8_t id) { data2 = id; }
    //void setTotalChunks(std::uint8_t chunks) { data3 = chunks; } // optional
    void setPayload(const std::array<std::uint8_t, 25>& data) { payload = data; }

};



