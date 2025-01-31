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

