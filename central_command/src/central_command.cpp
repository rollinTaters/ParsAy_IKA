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

#include "central_command.hpp"
#include <array>
#include <iostream>
#include <thread>   // this_thread::sleep_for

bool drive_auto_mode_enabled = false;
bool turret_auto_mode_enabled = false;

CommsModule ccm_comms( CommsModule::udp, CommsModule::command_channel );
//CommsModule ccm_comms2( CommsModule::udp, CommsModule::ngc_channel );

// lets define some parkour parameters
std::array<OperationMode, 1> legs =
{
    OperationMode{ 
            5,
            3,
            1 }
};


int main()
{
    // what now..?

    DPacket pack1;
    pack1.data1 = 0;
    pack1.data2 = 0;

    while( true )
    {
        std::this_thread::sleep_for( std::chrono::seconds(1) );
        if(ccm_comms.sendPacket( pack1, CommsModule::ngc_channel ))
        {
            std::cout<<"CCM: packet sent succesfully\n";
        }
        pack1.data1 += 1;
        pack1.data2 += 2;
    }
}

