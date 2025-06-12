#pragma once
#include "Observer.h"
#include "GameObject.h"
#include "EnemyComponent.h"

namespace dae
{
    class CherryPowerUp final : public Observer
    {
    public:
        CherryPowerUp(GameObject* owner);
        void OnNotify(const GameObject& gameObject, Event event) override;
    private:
        GameObject* m_Owner;
        EnemyComponent* m_pEnemyComponent;
        bool m_IsCollected;
        RenderComponent* m_pCherryRenderComponent = nullptr;
    };
}