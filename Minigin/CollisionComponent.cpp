#include "CollisionComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include <iostream>

namespace dae
{
    CollisionComponent::CollisionComponent(GameObject* owner, float width, float height , Scene* scene)
        : Component(owner), m_Width(width), m_Height(height), m_Scene(scene)
    {
    }

    void CollisionComponent::Update(float /*deltaTime*/)
    {
        auto gameObjects = m_Scene->GetGameObjects();

        for (auto& objects : gameObjects)
        {
            if (objects.get() == GetOwner())
            {
                continue;
            }
            if (objects->HasComponent<CollisionComponent>())
            {
                if (IsOverlapping(*objects->GetComponent<CollisionComponent>()))
                {
                    NotifyObservers(*objects, EVENT_COLLISION);
                }
            }
        }
    }

    glm::vec2 CollisionComponent::GetPosition() const
    {
        auto pos = GetOwner()->GetComponent<Transform>()->GetWorldPosition();
        return { pos.x, pos.y };
    }

    glm::vec2 CollisionComponent::GetSize() const
    {
        return { m_Width, m_Height };
    }

    bool CollisionComponent::IsOverlapping(const CollisionComponent& other) const
    {
        auto aPos = GetPosition();
        auto aSize = GetSize();
        auto bPos = other.GetPosition();
        auto bSize = other.GetSize();

        bool overlapping = (aPos.x < bPos.x + bSize.x &&
            aPos.x + aSize.x > bPos.x &&
            aPos.y < bPos.y + bSize.y &&
            aPos.y + aSize.y > bPos.y);

        std::cout << "Checking AABB overlap:\n";
        std::cout << "A Pos: (" << aPos.x << ", " << aPos.y << "), Size: (" << aSize.x << ", " << aSize.y << ")\n";
        std::cout << "B Pos: (" << bPos.x << ", " << bPos.y << "), Size: (" << bSize.x << ", " << bSize.y << ")\n";
        std::cout << "Overlapping: " << (overlapping ? "Yes" : "No") << "\n";

        std::cout << "Player Pos: (" << aPos.x << ", " << aPos.y << ") -> Size: (" << aSize.x << ", " << aSize.y << ")\n";
        std::cout << "Collectible Pos: (" << bPos.x << ", " << bPos.y << ") -> Size: (" << bSize.x << ", " << bSize.y << ")\n";

        return overlapping;
    }

}

