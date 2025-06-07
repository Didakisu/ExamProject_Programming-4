#pragma once
#include "Component.h"
#include "Subject.h"
#include <iostream>

namespace dae
{
    class ScoreComponent : public Observer, public Component
    {
    public:
        ScoreComponent(GameObject* owner, int initialPoints );
        void Update(float /*deltaTime*/) override {}
        void OnNotify(const GameObject& gameObject, Event event) override;

        int GetPoints() const { return m_Points; }
    private:
        GameObject* m_pOwner;
        int m_Points{};
        int m_ConsecutiveGemsCollected = 0;
        int m_LiveCounter{};
    };
}