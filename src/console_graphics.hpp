
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

bool init_graphics( sf::RenderTarget* );


