#include "Gamepad.h"
#include <windows.h>
#include <XInput.h>

namespace dae {

    class Gamepad::GamepadImpl final {
    public:
        XINPUT_STATE m_State{};
        XINPUT_STATE m_PreviousState{};

        WORD m_ButtonsPressedThisFrame{};
        WORD m_ButtonsReleasedThisFrame{};
        unsigned int m_Index;  

        static constexpr int LEFT_THUMB_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        static constexpr int RIGHT_THUMB_DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

        explicit GamepadImpl(unsigned int index) : m_Index(index) {
            ZeroMemory(&m_State, sizeof(XINPUT_STATE));
            ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
        }

        bool Update() {
            memcpy(&m_PreviousState, &m_State, sizeof(XINPUT_STATE));
            DWORD result = XInputGetState(m_Index, &m_State);
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

    Gamepad::Gamepad(unsigned int index)
        : pImpl(std::make_unique<GamepadImpl>(index)) {}  

    Gamepad::~Gamepad() = default;  

    bool Gamepad::Update() {
        return pImpl->Update();
    }

    bool Gamepad::IsButtonPressed(GamePadButton button) const {
        return pImpl->IsButtonPressed(button);
    }

    bool Gamepad::IsButtonDown(GamePadButton button) const {
        return pImpl->IsButtonDown(button);
    }

    bool Gamepad::IsButtonUp(GamePadButton button) const {
        return pImpl->IsButtonUp(button);
    }
    //
    unsigned int Gamepad::GetIndex() const {
        return pImpl->m_Index; 
    }
    //

}
