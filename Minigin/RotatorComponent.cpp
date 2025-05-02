#include "RotatorComponent.h"
#include "GameObject.h"  
#include <SDL_stdinc.h>
#include <iostream>

namespace dae
{
    RotatorComponent::RotatorComponent(GameObject* owner, float radius, float speed)
        : Component(owner),
        m_Owner(owner),
        m_Radius(radius),
        m_Speed(speed),
        m_Angle(0.0f)
    {
    }

    void RotatorComponent::Update(float deltaTime)
    {
        //delta time could be added
        m_Angle += m_Speed * deltaTime;

        float x = m_Radius * cosf(m_Angle);
        float y = m_Radius * sinf(m_Angle);

        m_Owner->GetTransform()->SetLocalPosition(x, y, 0.f);
    }
}


