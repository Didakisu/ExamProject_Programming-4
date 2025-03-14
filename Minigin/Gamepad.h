#pragma once
#include <memory>

namespace dae {

    class Gamepad {
    public:
        explicit Gamepad(unsigned int index);
        ~Gamepad();

        bool Update();

        enum class GamePadButton
        {
            DPadLeft = 0x0004,
            DPadRight = 0x0008,
            DPadUp = 0x0001,
            DPadDown = 0x0002,
            Start = 0x0010,
            Back = 0x0020,
            LeftStick = 0x0040,
            RightStick = 0x0080,
            LeftShoulder = 0x0100,
            RightShoulder = 0x0200,
            X = 0x4000,
            B = 0x2000,
            A = 0x1000,
            Y = 0x8000
        };

        bool IsButtonPressed(Gamepad::GamePadButton button) const;
        bool IsButtonDown(Gamepad::GamePadButton button) const;
        bool IsButtonUp(Gamepad::GamePadButton button) const;

        //
        unsigned int GetIndex() const;
        //

    private:
        class GamepadImpl; 
        std::unique_ptr<GamepadImpl> pImpl;

        unsigned int m_Index;
    };
}
