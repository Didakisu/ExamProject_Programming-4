#include "Transform.h"

namespace dae
{
    Transform::Transform(GameObject* owner)
        : Component(owner)
    {
    }

    void Transform::SetPosition(float x, float y, float z)
    {
        m_position = { x, y, z };
    }
}







