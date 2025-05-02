#include "CollectibleManager.h"
#include "CollectibleComponent.h"

namespace dae
{
    void CollectibleManager::AddCollectible(GameObject* collectible)
    {
        m_Collectibles.push_back(collectible);
    }

    void CollectibleManager::Update(GameObject* player)
    {
        for (auto collectibleObj : m_Collectibles)
        {
            if (auto collectible = collectibleObj->GetComponent<CollectibleComponent>())
            {
                collectible->TryCollect(player);
            }
        }
    }
}