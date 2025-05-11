#include "CollectibleComponent.h"
#include "EventManager.h"
#include "Data.h"
#include <RenderComponent.h>
#include "CollisionComponent.h"
#include <iostream>

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
        //std::cout << "CollectibleComponent TryCollect called.\n";
        if (m_IsCollected) return;

        auto myCollision = GetOwner()->GetComponent<CollisionComponent>();
        auto playerCollision = player->GetComponent<CollisionComponent>();

        if (myCollision && playerCollision)
        {
            //std::cout << "Checking collision between collectible and player...\n"; // Debugging output
            if (myCollision->IsOverlapping(*playerCollision))
            {
                //std::cout << "Collision detected! Collectible will be collected.\n";
                OnCollected();
            }
            else
            {
                //std::cout << "No collision detected.\n";
            }
        }
        else
        {
            //std::cout << "Collision component missing on either collectible or player.\n";
        }
    }

}
