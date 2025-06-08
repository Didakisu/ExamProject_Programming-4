#pragma once
#include "Component.h"
#include "Subject.h"
#include <iostream>
#include "GameController.h"

namespace dae
{
    class ScoreComponent : public Observer, public Component
    {
    public:
        ScoreComponent(GameObject* owner, int& sharedScore);
        void Update(float /*deltaTime*/) override {}
        void OnNotify(const GameObject& gameObject, Event event) override;

        int GetPoints() const { return m_Points; }
        void SetTotalGemsInLevel(int totalGems) { m_TotalGemsInLevel = totalGems; }
    private:
        GameObject* m_pOwner;
        int m_ConsecutiveGemsCollected = 0;
        int m_LiveCounter{};
        int m_TotalGemsInLevel{ 0 };
        int m_TotalGemsCollected{ 0 };

        int& m_Points;
    };
}