#include "GoldBagComponent.h"
#include "GoldBagState.h"
#include "CollisionComponent.h"
#include "CollectibleComponent.h"
#include "GameObject.h"
#include <iostream>
#include "TileMap.h" 
#include "GoldBagStates.h"
#include <RenderComponent.h>

namespace dae
{
    GoldBagComponent::GoldBagComponent(GameObject* owner)
        : Component(owner)
    {
    }

    void GoldBagComponent::Update(float deltaTime)
    {
        if (m_CurrentState)
        {
            m_CurrentState->Update(*this, deltaTime);
        }
    }

    void GoldBagComponent::SetState(std::unique_ptr<GoldBagState> newState)
    {
        if (m_CurrentState)
            m_CurrentState->OnExit(*this);

        m_CurrentState = std::move(newState);

        if (m_CurrentState)
            m_CurrentState->OnEnter(*this);

        if (dynamic_cast<GoldBagFallingState*>(m_CurrentState.get()))
        {
            SetFallAnimationTimer(2.0f);
        }
    }

    bool GoldBagComponent::IsGroundBelow() const
    {
        if (!m_pTileMap) return true;  

        auto transform = GetOwner()->GetComponent<Transform>();
        if (!transform) return true;  

        glm::vec3 pos = transform->GetLocalPosition();

        int tileX = static_cast<int>(pos.x) / TileMap::TILE_WIDTH;
        int tileY = static_cast<int>(pos.y) / TileMap::TILE_HEIGHT;

        TileType below = m_pTileMap->GetTile(tileX, tileY + 1);
        return below != TileType::Empty; 
    }

    void GoldBagComponent::Fall(float deltaTime)
    {
        auto transform = GetOwner()->GetComponent<Transform>();
        if (transform)
        {
            auto pos = transform->GetLocalPosition();

            //if the bag already landed, no more fall!
            if (m_HasHitGround)
            {
                return;
            }

            //sthe fall start tile Y when falling begins
            if (m_FallStartTileY == -1) 
            {
                m_FallStartTileY = static_cast<int>(pos.y) / TileMap::TILE_HEIGHT;
            }

            if (IsGroundBelow())
            {
                m_HasHitGround = true;
                return;
            }

            //continue falling if no ground detected
            pos.y += m_FallSpeed * deltaTime;
            transform->SetLocalPosition(pos.x, pos.y, pos.z);
        }
    }

    bool GoldBagComponent::HasLanded() const
    {
        return m_HasHitGround;
    }

    void GoldBagComponent::Break()
    {
        std::cout << "GoldBagComponent: Breaking animation or sound here...\n";
    }

    void GoldBagComponent::MakeCollectible()
    {
        //GetOwner()->AddComponent<CollectibleComponent>();
    }

    void dae::GoldBagComponent::SetTileMap(TileMap* tileMap)
    {
        m_pTileMap = tileMap;
    }

    void GoldBagComponent::ResetHasHitGround()
    {
        m_HasHitGround = false;
        std::cout << "GoldBagComponent: Ground reset, can fall again.\n";
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
}
