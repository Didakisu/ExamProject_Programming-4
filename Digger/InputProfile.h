#pragma once
#include <SDL.h>
#include "Gamepad.h"

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

    struct GamepadProfile
    {
        Gamepad::GamePadButton up{};
        Gamepad::GamePadButton down{};
        Gamepad::GamePadButton left{};
        Gamepad::GamePadButton right{};
        Gamepad::GamePadButton fire{};
    };
}

