#pragma once
#include "Component.h"

namespace dae
{
    enum class Direction
    {
        Right,
        Left,
        Up,
        Down
    };

    class DirectionComponent : public Component
    {
    public:
        DirectionComponent(GameObject* owner);

        void SetDirection(Direction dir);
        Direction GetDirection() const;

    private:
        Direction m_Direction{ Direction::Right }; // Default direction
    };
}

