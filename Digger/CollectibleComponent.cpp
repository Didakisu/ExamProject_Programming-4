#include "CollectibleComponent.h"
#include "EventManager.h"
#include "Data.h"
#include <RenderComponent.h>
#include "CollisionComponent.h"
#include <iostream>
#include "PlayerComponent.h"
#include "EnemyComponent.h"

namespace dae
{
    CollectibleComponent::CollectibleComponent(GameObject* owner)
        : Component(owner)
    {
    }

    void CollectibleComponent::OnCollected()
    {
        if (!m_IsCollected)
        {
            std::cout << "CollectibleComponent: OnCollected triggered!\n";
            m_IsCollected = true;

            EventManager::GetInstance().FireEvent(EVENT_PLAYER_COLLECT_ITEM, nullptr, GetOwner());
            GetOwner()->MarkForDestruction();
        }
    }

    void CollectibleComponent::Update(float /*deltaTime*/)
    {

    }

    void CollectibleComponent::OnNotify(const GameObject& gameObject, Event event)
    {
        if (event == EVENT_COLLISION)
        {
            TryCollect(&gameObject);
        }
    }

    void CollectibleComponent::TryCollect(const GameObject* player)
    {
        if (m_IsCollected) return;

        if (!player->HasComponent<PlayerComponent>() && !player->HasComponent<EnemyComponent>())
            return;

        auto myCollision = GetOwner()->GetComponent<CollisionComponent>();
        auto playerCollision = player->GetComponent<CollisionComponent>();

        if (myCollision && playerCollision)
        {
            if (myCollision->IsOverlapping(*playerCollision))
            {
                OnCollected();
            }
            else
            {
                std::cout << "no collision detected." << std::endl;
            }
        }
    }

}
