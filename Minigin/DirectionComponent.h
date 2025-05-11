#pragma once
#include "Component.h"
#include <glm.hpp>

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
        glm::vec2 GetDirectionVector();

    private:
        Direction m_Direction{ Direction::Right }; // Default direction
    };
}

