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

            if (currentState) 
            {
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

            m_PreviousKeyboardState[binding.key] = currentState;
        }

        for(auto& [gamepad, bindings]: m_GamepadBindings)
        {
            if (gamepad->Update()) {
                bool gamepadButtonBeingProcessed = false;

                for (const auto& binding : bindings) {

                    if (!gamepadButtonBeingProcessed)
                    {
                        if (binding.state == InputState::Down && gamepad->IsButtonDown(static_cast<Gamepad::GamePadButton>(binding.button))) {
                            gamepadButtonBeingProcessed = true;
                            if (binding.command) binding.command->Execute(deltaTime);
                        }
                        else if (binding.state == InputState::Released && gamepad->IsButtonUp(static_cast<Gamepad::GamePadButton>(binding.button))) {
                            gamepadButtonBeingProcessed = true;
                            if (binding.command) binding.command->Execute(deltaTime);
                        }
                        else if (binding.state == InputState::Pressed && gamepad->IsButtonPressed(static_cast<Gamepad::GamePadButton>(binding.button))) {
                            gamepadButtonBeingProcessed = true;
                            if (binding.command) binding.command->Execute(deltaTime);
                        }
                    }
                }
            }
        }

        return true;
    }

    void InputManager::BindGamepadCommand(int gamepadId ,Gamepad::GamePadButton button, InputState state, std::unique_ptr<Command> command) 
    {
        if (gamepadId >= m_GamepadBindings.size())
        {
            m_GamepadBindings.resize(gamepadId + 1);
            m_GamepadBindings[gamepadId].first = std::make_unique<Gamepad>(gamepadId);
        }

        m_GamepadBindings[gamepadId].second.push_back({ button, state, std::move(command) });
    }

    void InputManager::BindKeyboardCommand(SDL_Scancode key, InputState state, std::unique_ptr<Command> command) 
    {
        m_KeyboardBindings.push_back({ key, state, std::move(command) });
    }

    void InputManager::UnbindGamepadCommand(int gamepadId ,Gamepad::GamePadButton button) 
    {
    m_GamepadBindings[gamepadId].second.erase(
        std::remove_if(m_GamepadBindings[gamepadId].second.begin(), m_GamepadBindings[gamepadId].second.end(),
            [button](const GamepadBinding& binding) {
                return binding.button == button;
            }),
        m_GamepadBindings[gamepadId].second.end()
    );
    }

    void InputManager::UnbindKeyboardCommand(SDL_Scancode key) 
    {
        m_KeyboardBindings.erase(
            std::remove_if(m_KeyboardBindings.begin(), m_KeyboardBindings.end(),
                [key](const KeyboardBinding& binding) {
                    return binding.key == key;
                }),
            m_KeyboardBindings.end()
        );
    }
}