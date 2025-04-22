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
namespace cg {
    static bool _initialized = false;
    Font m_font = LoadFont("./assets/fonts/arial.ttf");
    void InitWindowSafe(int width, int height, const char* title) {
        if (!_initialized) {
            InitWindow(width, height, title);
            SetTargetFPS(60);
            _initialized = true;  
        }
    }
    bool isInitialized() {
        return _initialized;
    }
    void createPanel(Vector2 pos, Vector2 size , Color color , const char* title){
        DrawTextEx(m_font, title, {pos.x, pos.y - 25}, 25, 1 ,RED);
        DrawRectangle(pos.x, pos.y , size.x, size.y, color);
    }



    Gauge* gauge_temp       = nullptr;
    Gauge* gauge_amp        = nullptr;
    Gauge* gauge_amp2       = nullptr;
    Gauge* gauge_temp2      = nullptr;
    Gauge* gauge_compass    = nullptr;
    Gauge* gauge_speed      = nullptr;
    Gauge* gauge_tachometer = nullptr;
    Adi*   gauge_adi        = nullptr;
    UserInput* input        = nullptr;
    Gauge* gauge_battery    = nullptr;
    Gauge* gauge_signal     = nullptr;

    void InitObjects() {
        EnsureWindow();
        gauge_temp    = new Gauge(Gauge::type_temperature, {565,65},   180.0f);
        gauge_amp     = new Gauge(Gauge::type_amp,         {760,65},   180.0f);
        gauge_amp2    = new Gauge(Gauge::type_amp,         {760,284},    180.0f);
        gauge_temp2   = new Gauge(Gauge::type_temperature, {565,284},  180.0f);
        gauge_speed   = new Gauge(Gauge::type_speedometer, {250,85},   200.0f);
        gauge_compass = new Gauge(Gauge::type_compass,     {400,650},  200.0f);
        gauge_tachometer = new Gauge(Gauge::type_tachometer, {50,85}, 200.0f);
        gauge_adi     = new Adi({150, 500},  300.0f);
        input         = new UserInput();
        gauge_battery = new Gauge(Gauge::type_battery, {30,10},100.0f);
        gauge_signal = new Gauge(Gauge::type_signal,{170,60},100.0f);
    }
    // -- DEBUG --
    float g_amp_val = 0.f;
    float g_temp_val = 0.f;
    float g_pitch_val = 0.f;
    float g_roll_val = 0.f;
    float g_heading_val = 0.f;
    float g_speed_val = 0.f;
    float g_battery_val = 0.f;
    float g_signal_val = 0.f;
    /*                   __
                        // \
                        \\_/ //
      ''-.._.-''-.._.. -(||)(')
                         '''
    */
   
   // Debug gauge test 
   
   void DEBUG_gauge_test()
   {
       // nudge value towards a random direction
       g_amp_val      += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
       g_temp_val     += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
       g_pitch_val    += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
       g_roll_val     += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
       g_heading_val  += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
       g_speed_val    += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
       g_battery_val  += ((rand()/float(RAND_MAX))-0.5f)*0.05f;
       g_signal_val   += ((rand()/float(RAND_MAX))-0.5f)*0.05f;

       // clamp value to be between 0 and 1
       g_amp_val     = std::max( 0.f, std::min( g_amp_val    , 1.f ));
       g_temp_val    = std::max( 0.f, std::min( g_temp_val   , 1.f ));
       g_pitch_val   = std::max( 0.f, std::min( g_pitch_val  , 1.f ));
       g_roll_val    = std::max( 0.f, std::min( g_roll_val   , 1.f ));
       g_heading_val = std::max( 0.f, std::min( g_heading_val, 1.f ));
       g_speed_val   = std::max( 0.f, std::min( g_speed_val, 1.f ));
       g_battery_val = std::max( 0.f, std::min( g_battery_val, 1.f ));
       g_signal_val = std::max( 0.f, std::min( g_signal_val, 1.f ));
       
       // update gauges
       gauge_amp->updateProportionalVal( g_amp_val );
       gauge_temp->updateProportionalVal( g_temp_val );
       gauge_battery->updateProportionalVal( g_battery_val );
       gauge_adi->updateRollVal_prop( g_roll_val );
       gauge_adi->updatePitchVal_prop( g_pitch_val );
       gauge_speed->updateProportionalVal( g_speed_val );
       gauge_compass->updateProportionalVal( g_heading_val );
       gauge_signal->updateProportionalVal( g_signal_val );
    }
}
    
// -- END OF DEBUG --
