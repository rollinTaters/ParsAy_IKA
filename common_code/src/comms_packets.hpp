#pragma once
#include <cstdint>
#include <cstring>  // memcpy
#include <array>
#include "utility.hpp"
//#include <iostream> // DEBUG

struct CommsPacket
{
    enum PacketType{
        undefined,

        drive_command,    // NOT IMPLEMENTED    CCM -> Drive Module
        drive_telemetry,  // NOT IMPLEMENTED    Drive Module -> CCM

        turret,             // CCM <-> turret module

        ngc_command,        // CCM -> NGC
        ngc_telemetry,      // NGC -> CCM

        video_packet,       // image processing -> console
        console_telemetry,  // CCM -> console
        console_command,    // console -> CCM

        request1,   // this is an idea
        request2,   // this is an idea
        request3    // this is an idea
    };


    // actual payload of the packet
    std::uint8_t packet_type = undefined;
    std::array< std::uint8_t, 31 > data{0};

    CommsPacket( PacketType t ):packet_type(t) {}
    CommsPacket():packet_type(undefined) {}


    private:
    void writeFloat_1( float input, int offset, float minval, float maxval )
    {
        // clamp it
        input = std::max( input, minval );
        input = std::min( input, maxval );

        // make the input fit between designated sizes value range
        float range = maxval - minval;
        std::uint8_t byte1 = (std::uint8_t)( input/range *255 );

        std::memcpy( &(data[offset]), &byte1, sizeof( byte1 ) );
    }

    float readFloat_1( int offset, float minval, float maxval ) const
    {
        std::uint8_t byte1;
        std::memcpy( &byte1, &(data[offset]), sizeof( byte1 ) );

        float range = maxval - minval;
        return ( byte1/255.f * range ) + minval;
    }

    void writeFloat_2( float input, int offset, float minval, float maxval )
    {
        // clamp it
        input = std::max( input, minval );
        input = std::min( input, maxval );

        // make the input fit between designated sizes value range
        float range = maxval - minval;
        std::uint16_t byte2 = (std::uint16_t)( input/range *65535 );

        std::memcpy( &(data[offset]), &byte2, sizeof( byte2 ) );
    }

    float readFloat_2( int offset, float minval, float maxval ) const
    {
        std::uint16_t byte2;
        std::memcpy( &byte2, &(data[offset]), sizeof( byte2 ) );

        float range = maxval - minval;
        return ( byte2/65535.f * range ) + minval;
    }

    void write_2( std::uint16_t input, int offset )
    {
        std::uint8_t b1 = (std::uint8_t)(0xFF & input);
        std::uint8_t b2 = (std::uint8_t)(input >> 8);
        data[offset] = b1;
        data[offset+1] = b2;
    }

    std::uint16_t read_2( int offset ) const
    {
        std::uint16_t b1 = data[offset];
        std::uint16_t b2 = data[offset+1];
        return ( b1 | (b2 << 8) );
    }
    public:

    bool isNull() const
    {
        // if there is data, its not null packet
        for( std::uint8_t d : data )
            if( d != 0 )
                return false;

        // if there is no data and packet type is undefined
        if( packet_type == undefined ) return true;

        // if there is no data, but packet type is defined,
        return false;
    }

/*
   These are different methods that we use to parse (encode/decode) the data in the packets.
   we use these packets to transfer data between different subsystems
   Feel free to add your own packet parser methods if needed
*/

    /* XXX there is no fucking drive module. electronics team is indecisive 
    // ---- data sent to the drive module ----
    float getSpeed(){ return data1; }
    float getSteer(){ return data2; }

    void setSpeed( float inp ){ data1 = inp; }
    void setSteer( float inp ){ data2 = inp; }

    // ---- data sent from drive module ----
    float getMotor1Temp(){ return data1; }
    float getMotor1Amps(){ return data2; }
    float getMotor1Vel() { return data3; }
    float getMotor2Temp(){ return data1; }
    float getMotor2Amps(){ return data2; }
    float getMotor2Vel() { return data3; }

    void setMotor1Temp( float inp ){ data1 = inp; }
    void setMotor1Amps( float inp ){ data2 = inp; }
    void setMotor1Vel ( float inp ){ data3 = inp; }
    void setMotor2Temp( float inp ){ data1 = inp; }
    void setMotor2Amps( float inp ){ data2 = inp; }
    void setMotor2Vel ( float inp ){ data3 = inp; }
    */



    /* TODO
    // ---- data sent/received to/from the turret module ----
    float getPan() { return data1; }
    float getTilt(){ return data2; }
    bool getLaserStatus(){ return data3; }

    void setPan ( float inp ){ data1 = inp; }
    void setTilt( float inp ){ data2 = inp; }
    void setLaserStatus( bool inp ){ data3 = inp; }
    */

    /* TODO
    // ---- data sent to the ngc module ----
    std::uint8_t getControlMode(){ return data1; }

    void setControlMode( std::uint8_t inp ){ data1 = inp; }
    */

    /* TODO
    // ---- NGC TELEMETRY: from NGC to CCM ----
    float getHeading(){ return data1; }
    float getPitch()  { return data2; }
    float getRoll()   { return data3; }

    void setHeading( float inp ){ data1 = inp; }
    void setPitch  ( float inp ){ data2 = inp; }
    void setRoll   ( float inp ){ data3 = inp; }
    */

    // ----  video data packet ----
    std::uint16_t getFrameID() const { return read_2( 0 ); }
    std::uint16_t getChunkID() const { return read_2( 2 ); }
    std::uint16_t getFrameSize() const { return read_2( 4 ); }  // this has one more byte to expand to
    //std::uint16_t getTotalChunks() const { return data3;} // optional
    std::array<std::uint8_t, 25> getPayload() const
    {
        std::array<std::uint8_t, 25> le_array;
        std::memcpy( &le_array, &(data[6]), sizeof(std::uint8_t)*25 );
        return le_array;
    }

    void setFrameID(std::uint16_t id) { write_2( id, 0 ); }
    void setChunkID(std::uint16_t id) { write_2( id, 2 ); }
    void setFrameSize(std::uint16_t size) { write_2( size, 4 ); }
    //void setTotalChunks(std::uint8_t chunks) { data3 = chunks; } // optional
    void setPayload(const std::array<std::uint8_t, 25>& inp_data)
    {
        std::memcpy( &(data[6]), &inp_data, sizeof(std::uint8_t)*25 );
    }

    // ---- Console Telemetry, from CCM to Console ----
    float ct_getMotor1Temp(){ return readFloat_1( 0, -20.f, 250.f ); }  // celsius
    float ct_getMotor2Temp(){ return readFloat_1( 1, -20.f, 250.f ); }  // celsius
    float ct_getMotor1Amps(){ return readFloat_1( 2, -10.f, 300.f ); }  // ampere
    float ct_getMotor2Amps(){ return readFloat_1( 3, -10.f, 300.f ); }  // ampere 
    float ct_getMotor1Vel() { return readFloat_1( 4, -5.f, 5.f ); }     // m/s
    float ct_getMotor2Vel() { return readFloat_1( 5, -5.f, 5.f ); }     // m/s
    float ct_getHeading(){ return readFloat_2(  6, 0.f, PI ); }      // radian
    float ct_getPitch()  { return readFloat_2(  8, -PI/2, PI/2 ); }  // radian
    float ct_getRoll()   { return readFloat_2( 10, -PI/2, PI/2 ); }  // radian
    float ct_getSpeed()  { return readFloat_2( 12, -5.f, 5.f ); }    // m/s


    void ct_setMotor1Temp( float inp ){ writeFloat_1( inp, 0, -20.f, 250.f ); } // celsius
    void ct_setMotor2Temp( float inp ){ writeFloat_1( inp, 1, -20.f, 250.f ); } // celsius
    void ct_setMotor1Amps( float inp ){ writeFloat_1( inp, 2, -10.f, 300.f ); } // ampere
    void ct_setMotor2Amps( float inp ){ writeFloat_1( inp, 3, -10.f, 300.f ); } // ampere
    void ct_setMotor1Vel ( float inp ){ writeFloat_1( inp, 4, -5.f, 5.f ); }    // m/s
    void ct_setMotor2Vel ( float inp ){ writeFloat_1( inp, 5, -5.f, 5.f ); }    // m/s
    void ct_setHeading( float inp ){ writeFloat_2( inp,  6, 0.f, PI ); }      // radian
    void ct_setPitch  ( float inp ){ writeFloat_2( inp,  8, -PI/2, PI/2 ); }  // radian
    void ct_setRoll   ( float inp ){ writeFloat_2( inp, 10, -PI/2, PI/2 ); }  // radian
    void ct_setSpeed  ( float inp ){ writeFloat_2( inp, 12, -5.f, 5.f ); }    // m/s   

    // ---- Console Command, from Console to CCM ----
    void setManualSpeed( float inp ){ writeFloat_2( inp, 0, -5.f, 5.f ); }  // m/s
    void setManualSteer( float inp ){ writeFloat_2( inp, 2, -1.f, 1.f );}   // unitless

    float getManualSpeed(){ return readFloat_2( 0, -5.f, 5.f ); }   // m/s
    float getManualSteer(){ return readFloat_2( 2, -1.f, 1.f ); }   // unitless

    // Camera commands for debugging purposes there won't be any camera in real life
    void setCamera(float inp) { writeFloat_2(inp, 14, -PI, PI); }
    float getCamera() const { return readFloat_2(14, -PI, PI); }

    // Mode commands
    void setHatMode(float inp) { writeFloat_2(inp, 16, -1.0f, 1.0f); }
    float getHatMode() const { return readFloat_2(16, -1.0f, 1.0f); }

    // Crosshair commands
    void setCrosshair(float inp) { writeFloat_2(inp, 18, -1.0f, 1.0f); }
    float getCrosshair() const { return readFloat_2(18, -1.0f, 1.0f); }

    // Turret commands
    void setTurret(float inp) { writeFloat_2(inp, 20, -PI, PI); }
    float getTurret() const { return readFloat_2(20, -PI, PI); }

    // Waypoint commands
    void setWaypoint(float inp) { writeFloat_2(inp, 22, -1.0f, 1.0f); }
    float getWaypoint() const { return readFloat_2(22, -1.0f, 1.0f); }


};

