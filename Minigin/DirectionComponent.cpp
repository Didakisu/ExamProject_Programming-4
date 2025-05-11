// DirectionComponent.cpp
#include "DirectionComponent.h"

namespace dae
{
    DirectionComponent::DirectionComponent(GameObject* owner)
        : Component(owner)
    {
    }

    void DirectionComponent::SetDirection(Direction dir)
    {
        m_Direction = dir;
    }

    Direction DirectionComponent::GetDirection() const
    {
        return m_Direction;
    }

    glm::vec2 DirectionComponent::GetDirectionVector()
    {
        switch (m_Direction)
        {
        case Direction::Right:
            return glm::vec2{ 1,0 };
        case Direction::Left:
            return glm::vec2{ -1,0 };
        case Direction::Up:
            return glm::vec2{ 0,-1 };
        case Direction::Down:
            return glm::vec2{ 0,1 };
        default: return glm::vec2{ 1,0 };
        }
    }
}
