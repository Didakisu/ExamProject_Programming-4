#include "EnemyComponent.h"
#include <Physics.h>
#include <iostream>
#include "EnemyStates.h"
#include <ctime>

namespace dae
{
    dae::EnemyComponent::EnemyComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap)
        : Component(owner), m_Scene(scene), m_pTileMap(tileMap)
    {

    }

    void dae::EnemyComponent::Initialize(const glm::vec3& startPosition)
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        m_pDirectionComponent = GetOwner()->AddComponent<DirectionComponent>();
        GetOwner()->AddComponent<RenderComponent>("enemy_sprites.png", 32, 32);

        m_pTransform = GetOwner()->AddComponent<Transform>();
        m_pTransform->SetLocalPosition(startPosition.x, startPosition.y, startPosition.z);

        m_pAnimationComponent = GetOwner()->AddComponent<AnimationComponent>();
        m_pCollisionComponent = GetOwner()->AddComponent<CollisionComponent>(32.f, 32.f, &m_Scene);

        AddState("Normal", std::make_unique<EnemyNormalState>(this));
        AddState("Enraged", std::make_unique<EnemyEnragedState>(this));
        SetInitialState("Normal");

        m_CurrentDirection = EnemyDirection::Down;//doesn't matter which way 
    }

    void dae::EnemyComponent::Update(float deltaTime)
    {
        StateMachine::Update(deltaTime);
        UpdateEnragedTimers(deltaTime);
    }

    std::string EnemyComponent::CheckNextState()
    {
       /* if (m_IsEnraged)
        {
            return "Enraged";
        }

        if (ShouldEnterEnragedState())
        {
            m_IsEnraged = true;
            m_EnragedTimer = 5.f; 
            return "Enraged";
        }*/

        return "Normal";
    }

    void EnemyComponent::HandleWalking(float deltaTime)
    {
        if (!m_pTransform || !m_pTileMap) return;
    
        UpdateTilePosition();
    
        bool snapped = CheckTileSnap();
        if (snapped)
        {
            EvaluateDirectionChange();
        }
    
        MoveTowardsNextTile(deltaTime);
    }


    void EnemyComponent::UpdateTilePosition()
    {
        glm::vec3 pos = m_pTransform->GetLocalPosition();

        m_TileX = static_cast<int>(round(pos.x / TileMap::TILE_WIDTH));
        m_TileY = static_cast<int>(round(pos.y / TileMap::TILE_HEIGHT));

        m_DesiredX = m_TileX * TileMap::TILE_WIDTH;//desired tile for snapping
        m_DesiredY = m_TileY * TileMap::TILE_HEIGHT;
    }

    bool EnemyComponent::CheckTileSnap()
    {
        glm::vec3 pos = m_pTransform->GetLocalPosition();

        const int epsilon = 4;
        int distanceX = abs(m_DesiredX - static_cast<int>(pos.x));
        int distanceY = abs(m_DesiredY - static_cast<int>(pos.y));

        bool atTileCenter = distanceX < epsilon && distanceY < epsilon;

        if (atTileCenter && (m_TileX != m_LastTileX || m_TileY != m_LastTileY))
        {
            pos.x = static_cast<float>(m_DesiredX);//snap the pos tothe center
            pos.y = static_cast<float>(m_DesiredY);
            m_pTransform->SetLocalPosition(pos.x, pos.y, pos.z);

            m_LastTileX = m_TileX;
            m_LastTileY = m_TileY;

            return true;
        }

        return false;
    }

    void EnemyComponent::EvaluateDirectionChange()
    {
        glm::vec2 dirVec = GetDirectionVector(m_CurrentDirection);

        int nextTx = m_TileX + static_cast<int>(dirVec.x);
        int nextTy = m_TileY + static_cast<int>(dirVec.y);

        TileType nextTile = m_pTileMap->GetTile(nextTx, nextTy);
        bool canContinue = IsWalkable(nextTile);

        if (!canContinue)
        {
            std::vector<EnemyDirection> options;

            AddMoveOptionIfValid(options, m_TileX, m_TileY - 1, EnemyDirection::Up, EnemyDirection::Down);
            AddMoveOptionIfValid(options, m_TileX, m_TileY + 1, EnemyDirection::Down, EnemyDirection::Up);
            AddMoveOptionIfValid(options, m_TileX - 1, m_TileY, EnemyDirection::Left, EnemyDirection::Right);
            AddMoveOptionIfValid(options, m_TileX + 1, m_TileY, EnemyDirection::Right, EnemyDirection::Left);

            if (!options.empty())
            {
                m_CurrentDirection = options[rand() % options.size()];
            }
            else
            {
                // No options found - try going backwards (opposite direction)
                m_CurrentDirection = GetOppositeDirection(m_CurrentDirection);
            }
        }
    }

    EnemyDirection EnemyComponent::GetOppositeDirection(EnemyDirection dir) const
    {
        switch (dir)
        {
        case EnemyDirection::Left:  return EnemyDirection::Right;
        case EnemyDirection::Right: return EnemyDirection::Left;
        case EnemyDirection::Up:    return EnemyDirection::Down;
        case EnemyDirection::Down:  return EnemyDirection::Up;
        default:                   return EnemyDirection::None;
        }
    }

    void EnemyComponent::AddMoveOptionIfValid(std::vector<EnemyDirection>& out, int x, int y, EnemyDirection dir, EnemyDirection opposite)
    {
        TileType tile = m_pTileMap->GetTile(x, y);
        if ((IsWalkable(tile)) && m_CurrentDirection != opposite)
        {
            out.push_back(dir);
        }
    }

    void EnemyComponent::MoveTowardsNextTile(float deltaTime)
    {
        if (m_CurrentDirection == EnemyDirection::None)
            return;

        glm::vec3 pos = m_pTransform->GetLocalPosition();
        glm::vec2 dirVec = GetDirectionVector(m_CurrentDirection);

        //move smoothly
        glm::vec2 nextPos = {
            pos.x + dirVec.x * m_Speed * deltaTime,
            pos.y + dirVec.y * m_Speed * deltaTime
        };

        float maxX = static_cast<float>(m_pTileMap->GetWidth() * TileMap::TILE_WIDTH - TileMap::TILE_WIDTH);//map bounds
        float maxY = static_cast<float>(m_pTileMap->GetHeight() * TileMap::TILE_HEIGHT);

        if (nextPos.x < 0.f || nextPos.y < 0.f || nextPos.x >= maxX || nextPos.y >= maxY)
            return;

        if (dae::Physics::CanMoveTo(GetOwner(), { nextPos.x, nextPos.y }, &m_Scene))
            m_pTransform->SetLocalPosition(nextPos.x, nextPos.y, pos.z);
    }

    glm::vec2 EnemyComponent::GetDirectionVector(EnemyDirection dir) const
    {
        switch (dir)
        {
        case EnemyDirection::Left:  return { -1, 0 };
        case EnemyDirection::Right: return { 1, 0 };
        case EnemyDirection::Up:    return { 0, -1 };
        case EnemyDirection::Down:  return { 0, 1 };
        default: return { 0, 0 };
        }
    }

    bool EnemyComponent::IsWalkable(TileType type)
    {
        return type == TileType::Empty || type == TileType::Hole;
    }











    void EnemyComponent::HandleEnragedBehavior(float deltaTime)
    {
        UpdateTilePosition();

        if (CheckTileSnap()) 
        {
            glm::vec2 dirVec = GetDirectionVector(m_CurrentDirection);
            int targetX = m_TileX + static_cast<int>(dirVec.x);
            int targetY = m_TileY + static_cast<int>(dirVec.y);
            TileType targetTile = m_pTileMap->GetTile(targetX, targetY);

            if (targetTile == TileType::Dirt)
            {
                m_pTileMap->SetTile(targetX, targetY, TileType::Empty);
            }
            else
            {
                m_IsEnraged = false;
                // No diggable tile ahead, maybe change direction or stop enraged state
            }
        }

        MoveTowardsNextTile(deltaTime);
    }

    void EnemyComponent::UpdateEnragedTimers(float deltaTime)
    {
        if (m_IsEnraged)
        {
            m_EnragedTimer -= deltaTime;
            if (m_EnragedTimer <= 0.f)
            {
                m_IsEnraged = false;
                m_EnragedCooldown = 5.f + static_cast<float>(rand() % 10); 
            }
        }
        else
        {
            m_EnragedCooldown -= deltaTime;
        }
    }

    bool EnemyComponent::ShouldEnterEnragedState() const
    {
        if (m_IsEnraged)
        {
            return false;
        }

        if (m_EnragedCooldown <= 0.f)
        {
            return (rand() % 100) < 10;
        }
        return false;
    }
}