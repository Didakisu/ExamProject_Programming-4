#pragma once
#include "Observer.h"

namespace dae
{
    class GameObject;

    class CollectibleComponent final : public Observer
    {
    public:
        CollectibleComponent(GameObject* owner, Event collectEvent);
        void OnNotify(const GameObject& gameObject, Event event) override;
    private:
        void OnCollected(GameObject* collector);

        GameObject* m_Owner; 
        Event m_CollectEvent;
        bool m_IsCollected{ false };
    };
}