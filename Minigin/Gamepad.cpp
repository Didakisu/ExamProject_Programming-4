#include "Gamepad.h"
#include <iostream>
#include <cmath> 
//these two should be in cpp not header!
#include "windows.h"
#include <XInput.h>

namespace dae {

    class Gamepad::GamepadImpl final {
    public:
        XINPUT_STATE m_State{};
        XINPUT_STATE m_PreviousState{};

        WORD m_ButtonsPressedThisFrame;
        WORD m_ButtonsReleasedThisFrame;

        static constexpr int LEFT_THUMB_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        static constexpr int RIGHT_THUMB_DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

        GamepadImpl() {
            ZeroMemory(&m_State, sizeof(XINPUT_STATE));
            ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
            m_ButtonsPressedThisFrame = 0;
            m_ButtonsReleasedThisFrame = 0;
        }

        bool Update(unsigned int index) {
            memcpy(&m_PreviousState, &m_State, sizeof(XINPUT_STATE));
            DWORD result = XInputGetState(index, &m_State);
            if (result != ERROR_SUCCESS) {
                return false;
            }

            WORD currentButtons = m_State.Gamepad.wButtons;
            WORD previousButtons = m_PreviousState.Gamepad.wButtons;

            m_ButtonsPressedThisFrame = (currentButtons & ~previousButtons);
            m_ButtonsReleasedThisFrame = (~currentButtons & previousButtons);

            return true;
        }

        bool IsButtonPressed(Gamepad::GamePadButton button) const {
            return (m_State.Gamepad.wButtons & static_cast<WORD>(button)) != 0;
        }

        bool IsButtonDown(Gamepad::GamePadButton button) const {
            return (m_ButtonsPressedThisFrame & static_cast<WORD>(button)) != 0;
        }

        bool IsButtonUp(Gamepad::GamePadButton button) const {
            return (m_ButtonsReleasedThisFrame & static_cast<WORD>(button)) != 0;
        }
    };

    Gamepad::Gamepad(unsigned int index) : m_Index(index) {
        pImpl = new GamepadImpl(); 
    }

    Gamepad::~Gamepad() {
        delete pImpl; 
    }

    bool Gamepad::Update() {
        return pImpl->Update(m_Index);
    }

    bool Gamepad::IsButtonPressed(Gamepad::GamePadButton button) const {
        return pImpl->IsButtonPressed(button); 
    }

    bool Gamepad::IsButtonDown(Gamepad::GamePadButton button) const {
        return pImpl->IsButtonDown(button); 
    }

    bool Gamepad::IsButtonUp(Gamepad::GamePadButton button) const {
        return pImpl->IsButtonUp(button); 
    }
}
