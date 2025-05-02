#pragma once
#include "Component.h"
#include "Subject.h"
#include <iostream>

namespace dae
{
    class HealthComponent : public Subject, public Component
    {
    public:
        HealthComponent(GameObject* owner, int initialLives);
        void Update(float /*deltaTime*/) override {}
        void LoseLife();

        int GetLives() const { return m_Lives; }
    private:
        GameObject* m_pOwner;
        int m_Lives;
    };
}
