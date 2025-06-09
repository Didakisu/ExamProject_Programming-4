#pragma once
#include <SDL.h>

namespace dae
{
    struct InputProfile
    {
        SDL_Scancode up{};
        SDL_Scancode down{};
        SDL_Scancode left{};
        SDL_Scancode right{};
        SDL_Scancode fire{};
    };
}

