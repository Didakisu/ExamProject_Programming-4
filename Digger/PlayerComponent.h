#pragma once
#include "Component.h"
#include "DirectionComponent.h"
#include "RenderComponent.h"
#include "AnimationComponent.h"
#include "Transform.h"
#include "CollisionComponent.h"
#include "ScoreComponent.h"
#include "InputManager.h"
#include "GameCommands.h"
#include "EventManager.h"
#include "Scene.h"
#include "TileMap.h"
#include "Observer.h"

namespace dae 
{
    class PlayerComponent;

    class PlayerObserver : public Observer
    {
    public:
        PlayerObserver(PlayerComponent* player)
            : m_pPlayer(player) {
        }

        void OnNotify(const GameObject& gameObject, Event event) override;

    private:
        PlayerComponent* m_pPlayer;
    };

    class PlayerComponent final : public Component
    {
    public:
        PlayerComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap);
        ~PlayerComponent() override = default;

        void Initialize(const glm::vec3& startPosition);
        void BindInput();

        void Update(float deltaTime) override;

        ScoreComponent* GetScoreComponent() const { return m_pScoreComponent; }
        bool IsDead() const { return m_IsDead; }
        void SetDead(bool isDead);
        void BouncingEffect(float deltaTime);

        //
        void TryFire();
    private:
        Scene& m_Scene;
        std::shared_ptr<TileMap> m_pTileMap;

        DirectionComponent* m_pDirectionComponent{};
        AnimationComponent* m_pAnimationComponent{};
        Transform* m_pTransform{};
        CollisionComponent* m_pCollisionComponent{};
        ScoreComponent* m_pScoreComponent{};

        bool m_IsDead{ false };

        float m_BounceTime{ 0.f };
        float m_BounceHeight{ 8.f };
        float m_BounceSpeed{ 5.f };
        glm::vec3 m_DeadOriginalPos{};

        float m_TotalBounceDuration{ 0.6f };
        bool m_HasPlayedRipAnimation{ false };

        float m_FireCooldown{ 0.f };
        const float m_FireCooldownDuration{ 1.f }; 
        dae::GameObject* m_pFireball{ nullptr };
    };
}