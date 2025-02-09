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


#include <iostream>
#include "console_graphics.hpp"



int main()
{
    std::cout<<"Unmanned Land Vehicle Command Console v0.1\n";

    // graphics initialization
    sf::RenderWindow window( sf::VideoMode(800,600), "Command Console" );
    window.setFramerateLimit(60);
    if( !init_graphics(&window) )
    {
        std::cerr<<"Failed to init graphics! Exiting.\n";
        return -1;
    }

    // main loop
    while( window.isOpen() )
    {
        // event processing
        sf::Event event;
        while( window.pollEvent(event) )
        {
            // request for closing
            if( event.type == sf::Event::Closed )
            {
                // do we wanna do any actions before closing?
                window.close();
            }
        }   // end of event processing

        //do stuff
        // a call to render stuff

        window.display();
    }


    std::cout<<"Exiting. Have a nice day\n";
}

