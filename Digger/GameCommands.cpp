#include "GameCommands.h"
#include "ScoreComponent.h"
#include "HealthComponent.h"
#include "EventManager.h"
#include "CollectibleComponent.h"
#include "SceneManager.h"
#include "Data.h"
#include <RenderComponent.h>
#include "DirectionComponent.h" 

void MoveCommand::Execute(float deltaTime)
{
    if (!m_pGameObject || !m_pGameObject->GetTransform()) return;

    glm::vec2 direction = m_Direction;

    if (direction.x != 0.0f && direction.y != 0.0f)
        direction.y = 0.0f;

    auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();

    currentPos.x += direction.x * m_Speed * deltaTime;
    currentPos.y += direction.y * m_Speed * deltaTime;

    m_pGameObject->GetTransform()->SetLocalPosition(currentPos.x, currentPos.y, currentPos.z);

    auto dirComponent = m_pGameObject->GetComponent<dae::DirectionComponent>();
    if (dirComponent)
    {
        if (direction.x > 0) dirComponent->SetDirection(dae::Direction::Right);
        else if (direction.x < 0) dirComponent->SetDirection(dae::Direction::Left);
        else if (direction.y < 0) dirComponent->SetDirection(dae::Direction::Up);
        else if (direction.y > 0) dirComponent->SetDirection(dae::Direction::Down);
    }
}

void KillPlayerCommand::Execute(float deltaTime)
{
    (void)deltaTime;
    if (m_GameObject)
    {
        auto healthComp = m_GameObject->GetComponent<dae::HealthComponent>();
        if (healthComp) {
            healthComp->LoseLife();
        }
    }
}

void CollectCommand::Execute(float /*deltaTime*/)
{
    //(void)deltaTime;

    //if (m_GameObject)
    //{
    //   /* auto scoreComp = m_GameObject->GetComponent<dae::ScoreComponent>();
    //    if (scoreComp)
    //    {
    //        scoreComp->PickUpCoin();
    //    }*/
    //    
    //    auto& eventManager = dae::EventManager::GetInstance();
    //    eventManager.FireEvent(EVENT_PLAYER_COLLECT_ITEM, nullptr , m_GameObject);
    //}


    if (!m_GameObject) return;

    for (auto collectibleObj : m_Collectibles)
    {
        if (auto collectible = collectibleObj->GetComponent<dae::CollectibleComponent>())
        {
            if (!collectible->IsCollected())
            {
                auto playerPos = m_GameObject->GetComponent<dae::Transform>()->GetWorldPosition();
                auto objPos = collectibleObj->GetComponent<dae::Transform>()->GetWorldPosition();

                float dist = glm::distance(glm::vec2(playerPos.x, playerPos.y), glm::vec2(objPos.x, objPos.y));
                if (dist < 50.f)
                {
                    collectible->OnCollected();
                    break;
                }
            }
        }
    }

}