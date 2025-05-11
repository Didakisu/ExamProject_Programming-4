#pragma once
#include "Component.h"
#include <memory>
#include "TileType.h"
#include "AnimationComponent.h"


class TileMap;

namespace dae
{
    class GoldBagState;

    class GoldBagComponent final : public Component
    {
    public:
        explicit GoldBagComponent(GameObject* owner);
        ~GoldBagComponent() override = default;

        void Update(float deltaTime) override;

        void SetState(std::unique_ptr<GoldBagState> newState);
        bool IsGroundBelow() const;
        void Fall(float deltaTime);
        bool HasLanded() const;
        void Break();
        void MakeCollectible();

        void SetTileMap(TileMap* tileMap);
        int GetFallDistance() const { return m_FallDistance; }
        void ResetHasHitGround();
        
        int GetFallStartTileY() const;
        void SetFallStartTileY(int tileY);
        int GetCurrentTileY() const;

        void SetFallAnimationTimer(float timer) { m_FallAnimationTimer = timer; }
        void DecreaseFallAnimationTimer(float deltaTime) { m_FallAnimationTimer -= deltaTime; }
        float GetFallAnimationTimer() const { return m_FallAnimationTimer; }
    private:
        std::unique_ptr<GoldBagState> m_CurrentState;
        float m_FallSpeed = 100.f;
        bool m_HasHitGround = false;

        TileMap* m_pTileMap = nullptr;
        int m_FallDistance = 0;

        int m_FallStartTileY{ -1 }; 
        float m_FallAnimationTimer{ 0.0f };

        AnimationComponent* m_pAnimationComponent = nullptr;
    };
}