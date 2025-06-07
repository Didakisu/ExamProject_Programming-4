#include "GoldBagComponent.h"
#include "GoldBagStates.h"
#include "CollisionComponent.h"
#include "CollectibleComponent.h"
#include "GameObject.h"
#include "TileMap.h"
#include <RenderComponent.h>
#include <iostream>
#include "PlayerComponent.h"
#include "EnemyComponent.h"

namespace dae
{
    GoldBagObserver::GoldBagObserver(GoldBagComponent* goldBagComp) :
        m_pGoldBagComponent(goldBagComp)
    {

    }


    void GoldBagObserver::OnNotify(const GameObject& gameObject, Event event)
    {
        if (event != EVENT_COLLISION)
            return;

        //enemy dies from falling bag

        auto playerTransform = gameObject.GetComponent<Transform>();
        auto bagTransform = m_pGoldBagComponent->GetOwner()->GetComponent<Transform>();

        

        if (!playerTransform || !bagTransform)
            return;

        glm::vec3 playerPos = playerTransform->GetWorldPosition();
        glm::vec3 bagPos = bagTransform->GetWorldPosition();

        float verticalDifference = std::abs(playerPos.y - bagPos.y);
        float maxVerticalOffset = 4.0f; 

        if (m_pGoldBagComponent->IsInFallingState() && verticalDifference > maxVerticalOffset)
        {
            if (gameObject.HasComponent<EnemyComponent>())
            {
                auto enemy = gameObject.GetComponent<EnemyComponent>();
                if (enemy)
                {
                    enemy->DieByFallingBag(m_pGoldBagComponent->GetOwner());
                    return;
                }
            }

            if (gameObject.HasComponent<PlayerComponent>())
            {
                auto player = gameObject.GetComponent<PlayerComponent>();
                if (player)
                {
                    player->SetDead(true);
                    return;
                }
            }
        }

        if (verticalDifference > maxVerticalOffset)
        {
            return; 
        }

        float horizontalDifference = playerPos.x - bagPos.x;
        float minHorizontalOffset = 4.0f; 
        if (std::abs(horizontalDifference) < minHorizontalOffset)
        {
            return; 
        }

        if (horizontalDifference > 0)
        {
            m_pGoldBagComponent->TryPush(PushDirection::Left); 
        }
        else
        {
            m_pGoldBagComponent->TryPush(PushDirection::Right); 
        }

    }

    GoldBagComponent::GoldBagComponent(GameObject* owner)
        : Component(owner)
    {
        InitializeAnimations();

        AddState("Resting", std::make_unique<GoldBagRestingState>(this));
        AddState("Falling", std::make_unique<GoldBagFallingState>(this));
        AddState("Shaking", std::make_unique<GoldBagShakingState>(this));
        AddState("Breaking", std::make_unique<GoldBagBreakingState>(this));
        AddState("Collectable", std::make_unique<GoldBagCollectibleState>(this));

        SetInitialState("Resting");
    }

    void GoldBagComponent::InitializeAnimations()
    {
        m_pAnimationComponent = GetOwner()->GetComponent<AnimationComponent>();
        if (m_pAnimationComponent)
        {
            m_pAnimationComponent->AddAnimation("Shaking", "goldBag_shaking.png", 32, 32, 2, 0.1f);
            m_pAnimationComponent->AddAnimation("Breaking", "Gold.png", 15, 18, 3, 0.1f);
            m_pAnimationComponent->AddAnimation("Falling", "GoldBagFalling.png", 32, 32, 1, 0.1f);
        }
    }

    void GoldBagComponent::Update(float deltaTime)
    {
        if (m_IsBeingPushed)
        {
            auto transform = GetOwner()->GetComponent<Transform>();
            if (!transform) return;

            glm::vec3 currentPos = transform->GetLocalPosition();
            glm::vec3 direction = glm::normalize(m_PushTargetPos - m_PushStartPos);
            glm::vec3 newPos = currentPos + direction * m_PushSpeed * deltaTime;

            if (glm::distance(m_PushStartPos, newPos) >= glm::distance(m_PushStartPos, m_PushTargetPos))
            {
                newPos = m_PushTargetPos;
                m_IsBeingPushed = false;
            }

            transform->SetLocalPosition(newPos.x, newPos.y, newPos.z);
            return; 
        }

        StateMachine::Update(deltaTime);

        auto* breakingState = dynamic_cast<GoldBagBreakingState*>(GetCurrentState());
        if (breakingState && breakingState->IsDone())
        {
            SwitchState("Collectable");
        }
    }

    std::string GoldBagComponent::CheckNextState()
    {
        if (m_IsBroken)
            return "";

        if (!IsInFallingState() && !IsGroundBelow())
        {
            if (GetFallAnimationTimer() < 0.f)
                return "Falling";
            else
            {
                if (!IsShakingAnimationPlaying())
                {
                    SetShakingAnimationPlaying(true);
                    SetFallStartTileY(GetCurrentTileY());
                }
                return "Shaking";
            }
        }

        if (HasLanded())
        {
            int distance = GetCurrentTileY() - GetFallStartTileY();
            m_FallDistance = distance;
            if (distance >= 3)
            {
                m_IsBroken = true;
                return "Breaking";
            }
            else
            {
                m_HasHitGround = false;
                return "Resting";
            }
        }

        return "";
    }

    bool GoldBagComponent::IsGroundBelow() const
    {
        if (!m_pTileMap) return true;

        auto transform = GetOwner()->GetComponent<Transform>();
        if (!transform) return true;

        glm::vec3 pos = transform->GetLocalPosition();
        int tileX = static_cast<int>(round(pos.x) / TileMap::TILE_WIDTH);
        int tileY = static_cast<int>(round(pos.y) / TileMap::TILE_HEIGHT);

        TileType below = m_pTileMap->GetTile(tileX, tileY + 1);
        return below != TileType::Empty && below != TileType::Hole;
    }

    void GoldBagComponent::Fall(float deltaTime)
    {
        auto transform = GetOwner()->GetComponent<Transform>();
        if (!transform) return;

        auto pos = transform->GetLocalPosition();

        if (m_HasHitGround) return;

        if (m_FallStartTileY == -1)
            m_FallStartTileY = static_cast<int>(pos.y) / TileMap::TILE_HEIGHT;

        if (IsGroundBelow())
        {
            int tileY = static_cast<int>(pos.y) / TileMap::TILE_HEIGHT * TileMap::TILE_HEIGHT;
            if (abs(tileY - pos.y) < 2)
            {
                m_HasHitGround = true;
                return;
            }
        }

        pos.y += m_FallSpeed * deltaTime;
        transform->SetLocalPosition(pos.x, pos.y, pos.z);
    }

    bool GoldBagComponent::HasLanded() const
    {
        return m_HasHitGround;
    }

    void GoldBagComponent::SetTileMap(TileMap* tileMap)
    {
        m_pTileMap = tileMap;
    }

    void GoldBagComponent::ResetHasHitGround()
    {
        m_HasHitGround = false;
    }

    int GoldBagComponent::GetFallStartTileY() const
    {
        return m_FallStartTileY;
    }

    void GoldBagComponent::SetFallStartTileY(int tileY)
    {
        m_FallStartTileY = tileY;
    }

    int GoldBagComponent::GetCurrentTileY() const
    {
        auto transform = GetOwner()->GetComponent<Transform>();
        if (!transform) return -1;

        glm::vec3 pos = transform->GetLocalPosition();
        return static_cast<int>(pos.y) / TileMap::TILE_HEIGHT;
    }

    bool GoldBagComponent::IsInFallingState() const
    {
        return dynamic_cast<GoldBagFallingState*>(const_cast<State*>(GetCurrentState())) != nullptr;
    }

    bool GoldBagComponent::IsInShakingState() const
    {
        return dynamic_cast<GoldBagShakingState*>(const_cast<State*>(GetCurrentState())) != nullptr;
    }

    bool GoldBagComponent::IsBroken() const
    {
        return m_IsBroken;
    }

    void GoldBagComponent::TryPush(PushDirection dir)
    {
        if (IsInFallingState() || IsInShakingState() || m_IsBroken || m_IsBeingPushed)
            return;

        auto transform = GetOwner()->GetComponent<Transform>();
        if (!transform || !m_pTileMap) return;

        glm::vec3 pos = transform->GetLocalPosition();
        m_PushStartPos = pos;

        const float offset = static_cast<float>(TileMap::TILE_WIDTH);

        glm::vec3 nextTargetPos;
        if (dir == PushDirection::Left)
        {
            nextTargetPos = pos + glm::vec3(-offset, 0.f, 0.f);
        }
        else if (dir == PushDirection::Right)
        {
            nextTargetPos = pos + glm::vec3(offset, 0.f, 0.f);
        }
        else
        {
            return;
        }

        float mapWidthPixels = static_cast<float>(m_pTileMap->GetWidth() * TileMap::TILE_WIDTH);
        float mapHeightPixels = static_cast<float>(m_pTileMap->GetHeight() * TileMap::TILE_HEIGHT);

        float bagWidth = static_cast<float>(TileMap::TILE_WIDTH);
        float bagHeight = static_cast<float>(TileMap::TILE_HEIGHT);

        if (nextTargetPos.x < 0.f)
        {
            nextTargetPos.x = 0.f;
        }
        else if (nextTargetPos.x + bagWidth > mapWidthPixels)
        {
            nextTargetPos.x = mapWidthPixels - bagWidth;
        }

        if (nextTargetPos.y < 0.f)
        {
            nextTargetPos.y = 0.f;
        }
        else if (nextTargetPos.y + bagHeight > mapHeightPixels)
        {
            nextTargetPos.y = mapHeightPixels - bagHeight;
        }

        if (glm::distance(pos, nextTargetPos) < 0.01f)
        {
            return;
        }

        m_PushTargetPos = nextTargetPos;
        m_IsBeingPushed = true;
    }
}