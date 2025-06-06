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
    CollectibleComponent::CollectibleComponent(GameObject* owner, int points)
        : Component(owner), m_Points(points)
    {
        //std::cout << "CollectibleComponent created and attached to GameObject: " << GetOwner() << std::endl;
    }

    void CollectibleComponent::OnCollected(GameObject* collector)
    {
        if (!m_IsCollected)
        {   
            std::cout << "CollectibleComponent: OnCollected triggered!\n";
            std::cout << "CollectibleComponent: OnCollected triggered! Owner address: " << GetOwner() << std::endl;
            m_IsCollected = true;

            EventManager::GetInstance().FireEvent(EVENT_PLAYER_COLLECT_ITEM, GetOwner(), collector);

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
                OnCollected(const_cast<GameObject*>(player));
            }
            else
            {
                std::cout << "no collision detected." << std::endl;
            }
        }
    }
}