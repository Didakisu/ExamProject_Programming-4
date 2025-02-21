#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae
{
    class Transform : public Component
    {
    public:
        explicit Transform(GameObject* owner);

        void SetPosition(float x, float y, float z);
        glm::vec3 GetPosition() const { return m_position; }

        void Update() override {}

    private:
        glm::vec3 m_position{};
    };
}




