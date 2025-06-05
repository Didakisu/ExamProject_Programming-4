#include "EnemyComponent.h"
#include <Physics.h>
#include <iostream>
#include "EnemyStates.h"
#include <ctime>
#include "LevelLoader.h"
#include <random>
#include <unordered_map>
#include "GoldBagStates.h"
#include "GoldBagComponent.h"
#include "EnemySpawner.h"
#include "Data.h"
#include "PlayerComponent.h"


namespace //anonymous namespace
{
    std::random_device rd;
    std::mt19937 rng(rd());

    struct DirectionMapping
    {
        glm::vec2 vector;
        dae::EnemyDirection opposite;
        dae::Direction asDirection;
    };

    const std::unordered_map<dae::EnemyDirection, DirectionMapping> g_DirectionMappings = 
    {
        { dae::EnemyDirection::Left,  { {-1, 0}, dae::EnemyDirection::Right, dae::Direction::Left  } },
        { dae::EnemyDirection::Right, { { 1, 0}, dae::EnemyDirection::Left,  dae::Direction::Right } },
        { dae::EnemyDirection::Up,    { { 0,-1}, dae::EnemyDirection::Down,  dae::Direction::Up    } },
        { dae::EnemyDirection::Down,  { { 0, 1}, dae::EnemyDirection::Up,    dae::Direction::Down  } }
    };
}

namespace dae
{
    dae::EnemyComponent::EnemyComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap)
        : Component(owner), m_Scene(scene), m_pTileMap(tileMap)
    {
    }

    void dae::EnemyComponent::Initialize(const glm::vec3& startPosition ,EnemySpawner* spawner)
    {
        m_pTransform = GetOwner()->GetComponent<Transform>();
        m_pTransform->SetLocalPosition(startPosition.x, startPosition.y, startPosition.z);

        m_pDirectionComponent = GetOwner()->AddComponent<DirectionComponent>();
        GetOwner()->AddComponent<RenderComponent>("enemy_sprites.png", 32, 32);

        m_pAnimationComponent = GetOwner()->AddComponent<AnimationComponent>();
        m_pCollisionComponent = GetOwner()->AddComponent<CollisionComponent>(32.f, 32.f, &m_Scene);

        AddObserver(std::make_unique<EnemySpawnerObserver>(spawner));

        AddState("Normal", std::make_unique<EnemyNormalState>(this));
        AddState("Enraged", std::make_unique<EnemyEnragedState>(this));
        AddState("Dead", std::make_unique<EnemyDeadState>(this));
        SetInitialState("Normal");

        m_CurrentDirection = EnemyDirection::Down;
    }


    void dae::EnemyComponent::Update(float deltaTime)
    {
        StateMachine::Update(deltaTime);
        UpdateEnragedTimers(deltaTime);
    }

    std::string EnemyComponent::CheckNextState()
    {
        if (m_IsDead)
        {
            return "Dead";
        }

        if (m_IsEnraged)
        {
            return "Enraged";
        }

        if (ShouldEnterEnragedState())
        {
            m_IsEnraged = true;
            m_EnragedTimer = 5.f;
            return "Enraged";
        }

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

        m_DesiredX = m_TileX * TileMap::TILE_WIDTH;
        m_DesiredY = m_TileY * TileMap::TILE_HEIGHT;
    }

    bool EnemyComponent::CheckTileSnap()
    {
        glm::vec3 pos = m_pTransform->GetLocalPosition();

        const int epsilon = 2;
        int distanceX = abs(m_DesiredX - static_cast<int>(pos.x));
        int distanceY = abs(m_DesiredY - static_cast<int>(pos.y));

        bool atTileCenter = distanceX < epsilon && distanceY < epsilon;

        if (atTileCenter && (m_TileX != m_LastTileX || m_TileY != m_LastTileY))
        {
            pos.x = static_cast<float>(m_DesiredX);
            pos.y = static_cast<float>(m_DesiredY);
            m_pTransform->SetLocalPosition(pos.x, pos.y, pos.z);

            m_LastTileX = m_TileX;
            m_LastTileY = m_TileY;

            return true;
        }

        return atTileCenter;
    }

    void EnemyComponent::EvaluateDirectionChange()
    {
        if(!dynamic_cast<EnemyEnragedState*>(GetCurrentState()))
        {
            std::vector<EnemyDirection> options;

            AddMoveOptionIfValid(options, m_TileX, m_TileY - 1, EnemyDirection::Up, EnemyDirection::Down);
            AddMoveOptionIfValid(options, m_TileX, m_TileY + 1, EnemyDirection::Down, EnemyDirection::Up);
            AddMoveOptionIfValid(options, m_TileX - 1, m_TileY, EnemyDirection::Left, EnemyDirection::Right);
            AddMoveOptionIfValid(options, m_TileX + 1, m_TileY, EnemyDirection::Right, EnemyDirection::Left);

            if (options.size() > 1)
            {
                std::uniform_int_distribution<size_t> dist(0, options.size() - 1);
                size_t idx = dist(rng);
                //std::cout << "random direction chosen" << idx << " out of " << options.size() << std::endl;
                m_CurrentDirection = options[idx];
            }
            else if (options.size() == 1)
            {
                m_CurrentDirection = options[0];
            }
            else
            {
                m_CurrentDirection = GetOppositeDirection(m_CurrentDirection);
            }
        }
        else
        {
            auto player = FindPlayer();
            if (player == nullptr)
            {
                return;
            }
            auto playerTransform = player->GetTransform()->GetWorldPosition();
            auto enemyTransform =GetOwner()->GetTransform()->GetWorldPosition();

            auto direction = playerTransform - enemyTransform;
            if (abs(direction.x) > abs(direction.y))
            {
                if (direction.x > 0)
                {
                    m_CurrentDirection = EnemyDirection::Right;
                }
                else
                {
                    m_CurrentDirection = EnemyDirection::Left;
                }
            }
            else 
            {
                if (direction.y > 0)
                {
                    m_CurrentDirection = EnemyDirection::Down;
                }
                else
                {
                    m_CurrentDirection = EnemyDirection::Up;
                }
            }
          
        }
    }

    EnemyDirection EnemyComponent::GetOppositeDirection(EnemyDirection dir) const
    {
        return g_DirectionMappings.at(dir).opposite;
    }

    glm::vec2 EnemyComponent::GetDirectionVector(EnemyDirection dir) const
    {
        return g_DirectionMappings.at(dir).vector;
    }

    Direction EnemyComponent::ConvertToDirection(EnemyDirection dir) const
    {
        return g_DirectionMappings.at(dir).asDirection;
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

        glm::vec2 nextPos = {
            pos.x + dirVec.x * m_Speed * deltaTime,
            pos.y + dirVec.y * m_Speed * deltaTime
        };

        float maxX = static_cast<float>(m_pTileMap->GetWidth() * TileMap::TILE_WIDTH - TileMap::TILE_WIDTH);
        float maxY = static_cast<float>(m_pTileMap->GetHeight() * TileMap::TILE_HEIGHT);

        if (nextPos.x < 0.f || nextPos.y < 0.f || nextPos.x >= maxX || nextPos.y >= maxY)
            return;

        if (dae::Physics::CanMoveTo(GetOwner(), { nextPos.x, nextPos.y }, &m_Scene))
            m_pTransform->SetLocalPosition(nextPos.x, nextPos.y, pos.z);
    }

    bool EnemyComponent::IsWalkable(TileType type)
    {
        if (dynamic_cast<EnemyEnragedState*>(GetCurrentState()))
        {
            return type == TileType::Empty || type == TileType::Hole || type == TileType::Dirt;
        }
        return type == TileType::Empty || type == TileType::Hole;
    }

    void EnemyComponent::HandleEnragedBehavior(float deltaTime)
    {
        if (!m_pTransform || !m_pTileMap) return;

        UpdateTilePosition();

        bool snapped = CheckTileSnap();
        if (snapped)
        {
            glm::vec2 dirVec = GetDirectionVector(m_CurrentDirection);

            auto transform = GetOwner()->GetTransform();
            glm::vec3 pos = transform->GetLocalPosition();

            int tx = static_cast<int>(round(pos.x / TileMap::TILE_WIDTH));
            int ty = static_cast<int>(round(pos.y / TileMap::TILE_HEIGHT));

            TileType targetTile = m_pTileMap->GetTile(tx, ty);

            if (targetTile != TileType::Empty && targetTile != TileType::Hole)
            {
                m_pTileMap->SetTile(tx, ty, TileType::Empty);

                float fx = static_cast<float>(tx * TileMap::TILE_WIDTH);
                float fy = static_cast<float>(ty * TileMap::TILE_HEIGHT);
                const float z = 1.f;

                Direction dir = ConvertToDirection(m_CurrentDirection);
                dae::LevelLoader::SpawnEmpty(m_Scene, fx, fy, z, dir);
            }

            EvaluateDirectionChange();
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
                std::uniform_int_distribution<int> cooldownDist(0, 9);
                int cooldownRand = cooldownDist(rng);
                m_EnragedCooldown = 5.f + static_cast<float>(cooldownRand);
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
            std::uniform_int_distribution<int> chanceDist(0, 99);
            int chance = chanceDist(rng);
            return chance < 1;
        }
        return false;
    }

    void EnemyComponent::Die()
    {
        if (m_pCollisionComponent)
        {
            m_pCollisionComponent->GetOwner()->RemoveComponent<CollisionComponent>();
            m_pCollisionComponent = nullptr;
        }
        m_IsDead = true;

        std::cout << "Enemy died, notifying observers...\n";
        NotifyObservers(*GetOwner(), EVENT_ENEMY_DIED);
    }

    void EnemyComponent::DieByFallingBag(GameObject* bag)
    {
        m_IsDead = true;
        m_BagToFollow = bag;
        m_FallingWithBag = true;
    }

    void EnemyComponent::HandleDeadBehavior()
    {
        if (!m_FallingWithBag)
        {
            GetOwner()->MarkForDestruction();
            return;
        }

        if (!m_BagToFollow)
        {
            GetOwner()->MarkForDestruction();
            return;
        }

        auto bagTransform = m_BagToFollow->GetComponent<Transform>();
        auto myTransform = GetOwner()->GetComponent<Transform>();
        if (bagTransform && myTransform)
        {
            auto bagPos = bagTransform->GetLocalPosition();
            const float yOffset = -(TileMap::TILE_HEIGHT/2);

            myTransform->SetLocalPosition(bagPos.x, bagPos.y + yOffset, bagPos.z);
        }

        auto bagComp = m_BagToFollow->GetComponent<GoldBagComponent>();
        if (bagComp)
        {
            if (bagComp->IsBroken())
            {
                m_FallingWithBag = false;
                GetOwner()->MarkForDestruction();
            }
            else
            {
                GetOwner()->MarkForDestruction();
            }
        }
    }

    void EnemyComponent::SetTileMap(std::shared_ptr<TileMap> tileMap)
    {
        m_pTileMap = tileMap;
    }

    GameObject* EnemyComponent::FindPlayer()
    {
       auto& gameObjects = GetScene().GetGameObjects();

       for (const auto& obj : gameObjects)
       {
           if (auto player = obj->GetComponent<PlayerComponent>() ; player && !player->IsDead()) 
           {
               
               return obj.get();
           }
       }
       return nullptr;
    }
}