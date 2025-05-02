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
}
