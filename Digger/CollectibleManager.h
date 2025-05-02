#pragma once
#include <vector>
#include "GameObject.h"

namespace dae
{
    class CollectibleManager
    {
    public:
        void AddCollectible(GameObject* collectible);
        void Update(GameObject* player);

    private:
        std::vector<GameObject*> m_Collectibles;
    };
}