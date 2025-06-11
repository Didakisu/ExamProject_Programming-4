#pragma once
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
#include "EnemySpawner.h"
#include "Observer.h"
#include <memory>


namespace dae
{
    class State;
    class StateMachine;
    class EnemySpawner; 
    class EnemyComponent;

    enum class EnemyDirection 
    { 
      Up,
      Down,
      Left,
      Right,
      None
    };


    class EnemyBonusObserver final : public Observer
    {
    public:
        EnemyBonusObserver(EnemyComponent* enemy);
        void OnNotify(const GameObject& sender, Event event) override;
    private:
        EnemyComponent* m_pEnemy;
    };



    class EnemyComponent final : public Component, public StateMachine ,public Subject
    {
    public:
        EnemyComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap);
        ~EnemyComponent() override = default;
        void Initialize(const glm::vec3& startPosition, EnemySpawner* spawner = nullptr);
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
        void ResetDeadState() { m_IsDead = false; }

        void SetTileMap(std::shared_ptr<TileMap> tileMap);

        Scene& GetScene() { return m_Scene; }
        const Scene& GetScene() const { return m_Scene; }
        GameObject* FindPlayer();
        void EndBonusState();

        void SetBonusStateActive(bool isActive);

    protected:
        std::string CheckNextState() override;
    private:
        Scene& m_Scene;
        std::shared_ptr<TileMap> m_pTileMap;

        DirectionComponent* m_pDirectionComponent{};
        AnimationComponent* m_pAnimationComponent{};
        Transform* m_pTransform{};
        CollisionComponent* m_pCollisionComponent{};
        std::shared_ptr<EnemyBonusObserver>m_Observers;

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

        bool m_IsInBonusState{ false };
    };
}