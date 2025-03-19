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


#include "console_graphics.hpp"


sf::RenderTarget* render_target = nullptr;;
bool graphics_initialized = false;

Gauge gauge_temp( Gauge::type_temperature, sf::Vector2f(300,50), 200 );
Gauge gauge_amp( Gauge::type_amp, sf::Vector2f(600,50), 200 );
Adi gauge_adi(sf::Vector2f(500,300), 400 );
Gauge gauge_compass ( Gauge::type_compass, sf::Vector2f(150,350),200);
UserInput input;

bool init_graphics( sf::RenderTarget* inp_rt )
{
    // TODO or not todo, that is the question

    // maybe create gauges here?

    render_target = inp_rt;
    graphics_initialized = true;
    return true;
}

// -- DEBUG --
float g_amp_val     = 0.f;
float g_temp_val    = 0.f;
/*                    __
                     // \
                     \\_/ //
   ''-.._.-''-.._.. -(||)(')
                     '''
*/                     
float g_pitch_val   = 0.f;
float g_roll_val    = 0.f;
float g_heading_val = 0.f;


void DEBUG_gauge_test()
{
    // nudge value towards a random direction
    g_amp_val      += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
    g_temp_val     += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
	g_pitch_val    += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
    g_roll_val     += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
    g_heading_val  += ((rand()/float(RAND_MAX))-0.5f)*0.05f;

    // clamp value to be between 0 and 1
    g_amp_val     = std::max( 0.f, std::min( g_amp_val    , 1.f ));
    g_temp_val    = std::max( 0.f, std::min( g_temp_val   , 1.f ));
	g_pitch_val   = std::max( 0.f, std::min( g_pitch_val  , 1.f ));
    g_roll_val    = std::max( 0.f, std::min( g_roll_val   , 1.f ));
    g_heading_val = std::max( 0.f, std::min( g_heading_val, 1.f ));



    // update gauges
    gauge_amp.updateProportionalVal( g_amp_val );
    gauge_temp.updateProportionalVal( g_temp_val );

	gauge_adi.updateRollVal_prop( g_roll_val );
	gauge_adi.updatePitchVal_prop( g_pitch_val );

	gauge_compass.updateProportionalVal( g_heading_val );
}
// -- END OF DEBUG --

