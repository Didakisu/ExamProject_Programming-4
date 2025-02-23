#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae
{
    class RotatorComponent : public Component
    {
    public:
        RotatorComponent(GameObject* owner, float radius, float speed);

        void Update() override;

    private:
        GameObject* m_Owner;         
        float m_Radius;           
        float m_Speed;            
        float m_Angle;            
    };
}

