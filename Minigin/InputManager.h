#pragma once
//
#include <SDL.h> 
//
#include "Singleton.h"
#include "Command.h"
#include <vector>
#include <memory>
#include "Gamepad.h"
#include <unordered_map>
#include <unordered_set>

namespace dae
{
    enum class InputState {
        Pressed,
        Released,
        Down
    };

    struct GamepadBinding {
        Gamepad::GamePadButton button;
        InputState state;
        std::unique_ptr<Command> command;
    };

    struct KeyboardBinding {
        SDL_Scancode key;
        InputState state;
        std::unique_ptr<Command> command;
    };

    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool ProcessInput(float deltaTime);

        void BindGamepadCommand( int gamepadId,Gamepad::GamePadButton button, InputState state, std::unique_ptr<Command> command);
        void BindKeyboardCommand(SDL_Scancode key, InputState state, std::unique_ptr<Command> command);

        void UnbindGamepadCommand(int gamepadId,Gamepad::GamePadButton button);
        void UnbindKeyboardCommand(SDL_Scancode key);
    private:
        std::vector<KeyboardBinding> m_KeyboardBindings;
        std::unordered_map<SDL_Scancode, bool> m_PreviousKeyboardState;

        std::vector<std::pair<std::unique_ptr<Gamepad>,std::vector<GamepadBinding>>> m_GamepadBindings;
    };
}