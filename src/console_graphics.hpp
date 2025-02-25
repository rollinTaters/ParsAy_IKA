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


/*
   This is where we display data from the vehicle and issue control commands

   - vehicle attitude indicator

   - vehicle control mode (manual, auto-drive, fully-auto)

   - camera feeds

   - turret attitude
   - weapon control (target aquisition status, big red fire button)

   - motor loading
   - component temperatures
   - land speed
   - direction of travel

   - overhead map
   - waypoints display

   - immediate obstacle display


   */

#pragma once
#include "SFML/Graphics.hpp"
#include "gauge.hpp"

extern Gauge gauge_temp;
extern Gauge gauge_amp;

bool init_graphics( sf::RenderTarget* );

void DEBUG_gauge_test();

