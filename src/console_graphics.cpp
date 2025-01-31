#include "console_graphics.hpp"


sf::RenderTarget* render_target = nullptr;;
bool graphics_initialized = false;

bool init_graphics( sf::RenderTarget* inp_rt )
{
    // TODO or not todo, that is the question

    // maybe create gauges here?

    render_target = inp_rt;
    graphics_initialized = true;
    return true;
}
