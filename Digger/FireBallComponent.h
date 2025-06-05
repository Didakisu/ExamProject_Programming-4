#pragma once
#include "Component.h"
#include "Scene.h"
#include "Transform.h"
#include "AnimationComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "Observer.h"
#include "TileMap.h"

namespace dae
{
    class FireBallComponent final : public Component
    {
    public:
        FireBallComponent(GameObject* owner, Scene& scene, const glm::vec2& direction, TileMap* tileMap);
        void Update(float deltaTime) override;
    private:
        Scene& m_Scene;
        Transform* m_pTransform{};
        AnimationComponent* m_pAnimationComponent{};
        CollisionComponent* m_pCollisionComponent{};

        glm::vec2 m_Direction{};
        float m_Speed{ 150.f };

        TileMap* m_pTileMap = nullptr;
    };
}