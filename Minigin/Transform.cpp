#include "Transform.h"
#include <iostream>
#include "GameObject.h"

namespace dae
{
    Transform::Transform(GameObject* owner)
        : Component(owner)
    {
    }

    void Transform::SetPosition(float x, float y, float z)
    {
        glm::vec3 parentWorldPosition = { 0 , 0 , 0 };
        if (m_pOwner->GetParent())
        {
           parentWorldPosition = m_pOwner->GetParent()->GetTransform()->GetWorldPosition();
        }
        glm::vec3 originalWorldPos = glm::vec3{ x , y , z };
        glm::vec3 newLocalPosition = originalWorldPos - parentWorldPosition;
        

        SetLocalPosition(newLocalPosition.x , newLocalPosition.y , newLocalPosition.z);
    }

    void Transform::SetLocalPosition(float x, float y, float z)
    {
        m_localPosition = { x, y, z };
        SetPositionDirty();
    }

    const glm::vec3& Transform::GetWorldPosition()
    {
        if (m_isPositionDirty)
        {
            UpdateWorldPosition();
        }
        return m_worldPosition;
    }

    void Transform::UpdateWorldPosition()
    {
        if (m_isPositionDirty)
        {
            if (m_pOwner->GetParent() == nullptr)
            {
                m_worldPosition = m_localPosition;
            }
            else
            {
                m_worldPosition = m_pOwner->GetParent()->GetTransform()->GetWorldPosition() + m_localPosition;
            }

            m_isPositionDirty = false;
        }
    }
}







