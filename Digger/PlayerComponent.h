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

namespace dae {

    class PlayerComponent final : public Component {
    public:
        PlayerComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap);
        ~PlayerComponent() override = default;

        void Initialize(const glm::vec3& startPosition);
        void BindInput();

        void Update(float deltaTime) override;

        ScoreComponent* GetScoreComponent() const { return m_pScoreComponent; }

    private:
        Scene& m_Scene;
        std::shared_ptr<TileMap> m_pTileMap;

        DirectionComponent* m_pDirectionComponent{};
        AnimationComponent* m_pAnimationComponent{};
        Transform* m_pTransform{};
        CollisionComponent* m_pCollisionComponent{};
        ScoreComponent* m_pScoreComponent{};
    };
}