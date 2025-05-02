#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{

    class CollectibleComponent final : public Component, public Observer
    {
    public:
        CollectibleComponent(GameObject* owner);
        void Update(float /*deltaTime*/) override;
        void OnCollected();
        bool IsCollected() const { return m_IsCollected; }
        int GetPoints() const { return m_Points; }

        void OnNotify(const GameObject& gameObject, Event event) override;

        void TryCollect(const GameObject* other); 
    private:
        bool m_IsCollected{ false };
        int m_Points{ 0 };
    };
}



