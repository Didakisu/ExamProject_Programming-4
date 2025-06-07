#include "CollectibleComponent.h"
#include "EventManager.h"
#include "Data.h"
#include <iostream>
#include "PlayerComponent.h"
#include "EnemyComponent.h"

namespace dae
{
    CollectibleComponent::CollectibleComponent(GameObject* owner, Event collectEvent)
        : m_Owner(owner), m_CollectEvent(collectEvent)
    {
    }

    void CollectibleComponent::OnNotify(const GameObject& gameObject, Event event)
    {
        if (event == EVENT_COLLISION)
        {
            const GameObject* other = &gameObject;

            if (m_IsCollected) return;

            if (!other->HasComponent<PlayerComponent>() && !other->HasComponent<EnemyComponent>())
                return;

            OnCollected(const_cast<GameObject*>(other));
        }
    }

    void CollectibleComponent::OnCollected(GameObject* collector)
    {
        if (m_IsCollected) return;
        m_IsCollected = true;
        std::cout << "CollectibleComponent: collected by " << collector << std::endl;
        EventManager::GetInstance().FireEvent(m_CollectEvent, m_Owner, collector);
        m_Owner->MarkForDestruction();
    }
}
