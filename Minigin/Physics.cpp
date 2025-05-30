#include "Physics.h"
#include "CollisionComponent.h"

bool dae::Physics::CanMoveTo(GameObject* movingObj, glm::vec2 newPos, Scene* scene)
{
    if (!movingObj || !scene) return false;

    auto collision = movingObj->GetComponent<dae::CollisionComponent>();
    if (!collision) return true;

    auto transform = movingObj->GetTransform();
    auto oldPos = transform->GetLocalPosition();
    transform->SetLocalPosition(newPos.x, newPos.y, oldPos.z); // Temp move

    bool blocked = false;
    for (auto& obj : scene->GetGameObjects())
    {
        if (obj.get() == movingObj) continue;

        auto otherCol = obj->GetComponent<dae::CollisionComponent>();
        if (otherCol && !otherCol->IsTrigger())
        {
            if (collision->IsOverlapping(*otherCol))
            {
                otherCol->NotifyObservers(*movingObj, EVENT_COLLISION);
                collision->NotifyObservers(*movingObj, EVENT_COLLISION);
              blocked = true;
                break;
            }
        }
    }

    transform->SetLocalPosition(oldPos.x , oldPos.y , oldPos.z); // Revert
    return !blocked;
}
