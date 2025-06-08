#pragma once
#include "Component.h"
#include "Subject.h"
#include <iostream>


namespace dae
{
    class HealthComponent : public Subject, public Component , public Observer
    {
    public:
        HealthComponent(GameObject* owner, int initialLives);
        ~HealthComponent() override;
        void Update(float /*deltaTime*/) override {}
        void LoseLife();

        int GetLives() const { return m_Lives; }
        void OnNotify(const GameObject& gameObject, Event event) override;
    private:
        GameObject* m_pOwner;
        int m_Lives;
    };
}
