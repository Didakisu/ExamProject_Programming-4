#pragma once
#include "EnemyComponent.h"
#include "Component.h"
#include <glm.hpp>
#include "Scene.h"
#include "Transform.h"
#include "TileMap.h"
#include "CollisionComponent.h"
#include "DirectionComponent.h"
#include "AnimationComponent.h"
#include "RenderComponent.h"
#include "StateMachine.h"

namespace dae
{
    class State;
    class StateMachine;

    enum class EnemyDirection 
    { 
      Up,
      Down,
      Left,
      Right,
      None
    };

    class EnemyComponent final : public Component, public StateMachine
    {
    public:
        EnemyComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap);
        ~EnemyComponent() override = default;
        void Initialize(const glm::vec3& startPosition);
        void Update(float deltaTime) override;

        void HandleWalking(float deltaTime);
        bool IsWalkable(TileType type);

        void UpdateTilePosition();                     
        bool CheckTileSnap();                           
        void EvaluateDirectionChange();

        EnemyDirection GetOppositeDirection(EnemyDirection dir) const;
        Direction ConvertToDirection(EnemyDirection dir) const;
        glm::vec2 GetDirectionVector(EnemyDirection dir) const;

        void MoveTowardsNextTile(float deltaTime);                        
        
        void AddMoveOptionIfValid(std::vector<EnemyDirection>& out, int x, int y, EnemyDirection dir, EnemyDirection opposite); 
        void HandleEnragedBehavior(float deltaTime);
        void UpdateEnragedTimers(float deltaTime);
        bool ShouldEnterEnragedState() const;

        void Die();
        void DieByFallingBag(GameObject* bag);
        void HandleDeadBehavior(/*float *//*deltaTime*/);
    protected:
        std::string CheckNextState() override;
    private:
        Scene& m_Scene;
        std::shared_ptr<TileMap> m_pTileMap;

        DirectionComponent* m_pDirectionComponent{};
        AnimationComponent* m_pAnimationComponent{};
        Transform* m_pTransform{};
        CollisionComponent* m_pCollisionComponent{};

        EnemyDirection m_CurrentDirection{ EnemyDirection::None };
        EnemyDirection m_LastDirection{ EnemyDirection::None };
        float m_Speed{ 35.f };

        int m_TileX{};
        int m_TileY{};       
        int m_DesiredX{};
        int m_DesiredY{};   
        int m_LastTileX{ -1 };
        int m_LastTileY{ -1 };

        bool m_IsEnraged{false};
        float m_EnragedCooldown{0.f};
        float m_EnragedTimer{0.f};

        bool m_IsDead{ false };
        bool m_FallingWithBag{ false };
        GameObject* m_BagToFollow{ nullptr };
    };
}