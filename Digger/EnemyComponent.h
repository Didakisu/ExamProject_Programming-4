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

        void MoveTowardsNextTile(float deltaTime);                        
        glm::vec2 GetDirectionVector(EnemyDirection dir) const; 
        void AddMoveOptionIfValid(std::vector<EnemyDirection>& out, int x, int y, EnemyDirection dir, EnemyDirection opposite); 

        void HandleEnragedBehavior(float deltaTime);
        void UpdateEnragedTimers(float deltaTime);
        bool ShouldEnterEnragedState() const;
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
        float m_Speed{ 40.f };

        int m_TileX{};
        int m_TileY{};       
        int m_DesiredX{};
        int m_DesiredY{};   
        int m_LastTileX{ -1 };
        int m_LastTileY{ -1 };

        bool m_IsEnraged{false};
        float m_EnragedCooldown{0.f};
        float m_EnragedTimer{0.f};
    };
}