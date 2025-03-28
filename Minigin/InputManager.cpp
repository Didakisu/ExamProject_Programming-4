#include <SDL.h>
#include "InputManager.h"
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

        for (const auto& gamepad : m_Gamepads) {
            if (gamepad->Update()) {
                for (const auto& binding : m_GamepadBindings) {
                    if (binding.controllerIndex == gamepad->GetIndex()) {
                        if (binding.state == InputState::Down && gamepad->IsButtonDown(binding.button)) {
                            if (binding.command) binding.command->Execute(deltaTime);
                        }
                        else if (binding.state == InputState::Released && gamepad->IsButtonUp(binding.button)) {
                            if (binding.command) binding.command->Execute(deltaTime);
                        }
                        else if (binding.state == InputState::Pressed && gamepad->IsButtonPressed(binding.button)) {
                            if (binding.command) binding.command->Execute(deltaTime);
                        }
                    }
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

    void InputManager::UnbindGamepadCommand(Gamepad::GamePadButton button) {
        m_GamepadBindings.erase(
            std::remove_if(m_GamepadBindings.begin(), m_GamepadBindings.end(),
                [button](const GamepadBinding& binding) {
                    return binding.button == button;
                }),
            m_GamepadBindings.end()
        );
    }

    void InputManager::UnbindKeyboardCommand(SDL_Scancode key) {
        m_KeyboardBindings.erase(
            std::remove_if(m_KeyboardBindings.begin(), m_KeyboardBindings.end(),
                [key](const KeyboardBinding& binding) {
                    return binding.key == key;
                }),
            m_KeyboardBindings.end()
        );
    }

    void InputManager::AddGamepad(unsigned int index) {
        m_Gamepads.push_back(std::make_unique<Gamepad>(index));
    }

}