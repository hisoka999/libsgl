#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>

namespace utils
{
    namespace color
    {

        constexpr SDL_Color hex2color(const uint32_t hex)
        {
            SDL_Color color = {0, 0, 0, 255};
            color.r = 0xff0000 & hex;
            color.g = 0xff00 & hex;
            color.b = 0xff & hex;
            color.a = 255;
            return color;
        }

        constexpr uint32_t color2hex(const SDL_Color &color)
        {
            return (color.r << 16) + (color.g << 8) + color.b;
        }

        const SDL_Color RED = {255, 0, 0, 255};
        const SDL_Color GREEN = {0, 255, 0, 255};
        const SDL_Color BLUE = {0, 0, 255, 255};
        const SDL_Color WHITE = {255, 255, 255, 255};
        const SDL_Color BLACK = {0, 0, 0, 255};

    } // namespace color

} // namespace utils

#endif // COLOR_H
