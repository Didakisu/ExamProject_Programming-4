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

        bool keyBeingProcessed = false;

        for (const auto& binding : m_KeyboardBindings)
        {
            bool currentState = keyboardState[binding.key] != 0;

            if (currentState) {
                if (!keyBeingProcessed) {
                    keyBeingProcessed = true; 
                    if (binding.state == InputState::Down && !m_PreviousKeyboardState[binding.key]) {
                        if (binding.command) binding.command->Execute(deltaTime);
                    }
                    else if (binding.state == InputState::Pressed && currentState) {
                        if (binding.command) binding.command->Execute(deltaTime);
                    }
                    else if (binding.state == InputState::Released && !currentState && m_PreviousKeyboardState[binding.key]) {
                        if (binding.command) binding.command->Execute(deltaTime);
                    }
                }
            }

            m_PreviousKeyboardState[binding.key] = currentState;
        }

        if (m_Gamepad.Update()) {
            bool gamepadButtonBeingProcessed = false;

            for (const auto& binding : m_GamepadBindings) {
                bool currentButtonState = m_Gamepad.IsButtonDown(static_cast<Gamepad::GamePadButton>(binding.button));

                if (currentButtonState && !gamepadButtonBeingProcessed) {
                    gamepadButtonBeingProcessed = true; 
                    if (binding.state == InputState::Down && currentButtonState) {
                        if (binding.command) binding.command->Execute(deltaTime);
                    }
                    else if (binding.state == InputState::Released && !currentButtonState) {
                        if (binding.command) binding.command->Execute(deltaTime);
                    }
                    else if (binding.state == InputState::Pressed && currentButtonState) {
                        if (binding.command) binding.command->Execute(deltaTime);
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
}