#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <iostream>
#include "Gamepad.h"

namespace dae {

    bool InputManager::ProcessInput(float deltaTime) 
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                return false;
            }

        }
        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

        for (const auto& binding : m_KeyboardBindings) 
        {
            bool currentState = keyboardState[binding.key] != 0; 

            if (binding.state == InputState::Down && currentState && !m_PreviousKeyboardState[binding.key]) {
                if (binding.command) binding.command->Execute(deltaTime); 
            }

            else if (binding.state == InputState::Pressed && currentState) {
                if (binding.command) binding.command->Execute(deltaTime);
            }

            else if (binding.state == InputState::Released && !currentState && m_PreviousKeyboardState[binding.key]) {
                if (binding.command) binding.command->Execute(deltaTime); 
            }

            m_PreviousKeyboardState[binding.key] = currentState;
        }
             //ImGui_ImplSDL2_ProcessEvent(&e); 

        if (m_Gamepad.Update()) {
            for (const auto& binding : m_GamepadBindings) {
                if (binding.state == InputState::Down && m_Gamepad.IsButtonDown(static_cast<Gamepad::GamePadButton>(binding.button))) 
                {
                    if (binding.command) binding.command->Execute(deltaTime);
                }
                else if (binding.state == InputState::Released && m_Gamepad.IsButtonUp(static_cast<Gamepad::GamePadButton>(binding.button))) 
                {
                    if (binding.command) binding.command->Execute(deltaTime);
                }
                else if (binding.state == InputState::Pressed && m_Gamepad.IsButtonPressed(static_cast<Gamepad::GamePadButton>(binding.button))) 
                {
                    if (binding.command) binding.command->Execute(deltaTime);
                }
            }
        }

        return true;
    }

    void InputManager::BindGamepadCommand(Gamepad::GamePadButton button, InputState state, std::unique_ptr<Command> command) {
        m_GamepadBindings.push_back({ button, state, std::move(command) });
    }

    void InputManager::BindKeyboardCommand(SDL_Scancode key, InputState state, std::unique_ptr<Command> command) {
        m_KeyboardBindings.push_back({ key, state, std::move(command) });
    }
}