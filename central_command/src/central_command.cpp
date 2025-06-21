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
#include "serial_arduino.hpp"
#include "central_command.hpp"
#include "../../common_code/src/comms_module.hpp"
#include "../../common_code/src/comms_packets.hpp"
#include <array>
#include <iostream>
#include <thread>   // this_thread::sleep_for
#include <chrono> // seconds
bool drive_auto_mode_enabled = false;
bool turret_auto_mode_enabled = false;

CommsModule ccm_comms( CommsModule::udp, CommsModule::command_channel );

// lets define some parkour parameters
std::array<OperationMode, 4> legs =
{
    OperationMode{ 
            5,
            3,
            1 },
    OperationMode{ 
            5,
            3,
            1 },
    OperationMode{ 
            5,
            3,
            1 },
    OperationMode{ 
            5,
            3,
            1 }
};


int main()
{
    SerialArduino arduino("/dev/ttyUSB0", 9600);

    // what now..?

    // raw packet
    PacketBase raw_packet;

    // telemetry packets
    NGC_Telemetry_Packet ngctp;
    Drive_Telemetry_Packet1 dtp1;
    Drive_Telemetry_Packet2 dtp2;
    Turret_Packet ttp;  // turret telemetry packet

    // command packets
    NGC_Command_Packet ngccp;
    Turret_Packet tcp;  // turret control packet
    ImageStatus_Packet img_status{};

    std::cout << "CCM has been started. Arduino and UDP communication is active.\n";

    while( true )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500) );
        arduino.sendCommand("SPEED:100\n");

        // read received packets
        while( ccm_comms.packetAvailable() )
        {
            if (ccm_comms.readPacket(raw_packet))
            {
                switch (raw_packet.packet_type)
                {
                    case PacketType::ngc_command:
                        ngccp = *reinterpret_cast<NGC_Command_Packet*>(&raw_packet);
                        std::cout << "NGC command received\n";
                        break;

                    case PacketType::drive_telemetry1:
                        dtp1 = *reinterpret_cast<Drive_Telemetry_Packet1*>(&raw_packet);
                        std::cout << "Drive Telemetry 1 received\n";
                        break;

                    case PacketType::drive_telemetry2:
                        dtp2 = *reinterpret_cast<Drive_Telemetry_Packet2*>(&raw_packet);
                        std::cout << "Drive Telemetry 2 received\n";
                        break;

                    case PacketType::turret_telemetry:
                        ttp = *reinterpret_cast<Turret_Packet*>(&raw_packet);
                        std::cout << "Turret status received\n";
                        break;

                    case PacketType::image_status:
                        img_status = *reinterpret_cast<ImageStatus_Packet*>(&raw_packet);
                        std::cout << "Image processing status received\n";
                        break;

                    default:
                        std::cout << "Unknown packet received, type:" << static_cast<int>(raw_packet.packet_type) << "\n";
                        break;
                }
            }
            // TODO receive visual status from image processing module
            // TODO receive turret status
            // TODO receive drive motor status
            // TODO receive "pilot command" from console package
        }


        // TODO send "control mode" to ngc if necessary, otherwise send alive pulse
        // TODO send "turret control package" to turret, otherwise send alive pulse
        // TODO send pilot or "ngc drive command" to drive module

        // Örnek: Görüntü işleme sonucu taret kontrolü gerektiriyorsa
        if (img_status.detected_target)
        {
            tcp.yaw = img_status.target_yaw;
            tcp.pitch = img_status.target_pitch;
            tcp.fire = true;
            std::cout << "Hedef algılandı, taret yönlendiriliyor.\n";
        }
        else
        {
            tcp.fire = false; // Alive pulse
        }

        // Taret kontrol paketini gönder
        ccm_comms.sendPacket(tcp, CommsModule::turret_channel);

        // NGC komutu gönder (alive pulse veya komut)
        ccm_comms.sendPacket(ngccp, CommsModule::ngc_channel);

        // Arduino’ya da örnek veri gönderilebilir (duruma göre)
        arduino.sendCommand("LED:ON\n");

        // Konsola veri ilet (forward)
        ccm_comms.sendPacket(dtp1, CommsModule::console_channel);
        ccm_comms.sendPacket(dtp2, CommsModule::console_channel);
        ccm_comms.sendPacket(ngctp, CommsModule::console_channel);
        ccm_comms.sendPacket(ttp, CommsModule::console_channel);

        // Sahte verileri güncelle (test için)
        dtp1.data1 += 1;
        dtp1.data2 += 2;
        dtp1.data3 += 3;
        dtp2.data1 += 2;
        dtp2.data2 += 3;
        dtp2.data3 += 4;
        ngctp.data1 += 10;
        ngctp.data2 += 20;
        ngctp.data3 += 30;
        ttp.yaw += 0.1;
        ttp.pitch += 0.1;
    }
    return 0;
}

