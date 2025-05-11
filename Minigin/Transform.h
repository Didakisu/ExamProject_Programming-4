#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae
{
    class GameObject;

    class Transform : public Component
    {
    public:
        explicit Transform(GameObject* owner);

        void SetPosition(float x, float y, float z);
        void SetLocalPosition(float x, float y, float z);
        //glm::vec3 GetPosition() const { return m_position; }
        const glm::vec3& GetLocalPosition() { return m_localPosition; }
        const glm::vec3& GetWorldPosition();
        void UpdateWorldPosition();
        const void SetPositionDirty();
        bool IsPositionDirty() { return m_isPositionDirty; }

        void Update(float /*deltaTime*/) override {};

    private:
        glm::vec3 m_worldPosition{};
        glm::vec3 m_localPosition{};

        bool m_isPositionDirty{true}; //?
    };
}







