#include "FireBallComponent.h"
#include "EnemyComponent.h"
#include <iostream>
#include "VersusEnemyComponent.h"
#include "EventManager.h"
#include "Data.h"

namespace dae
{
    class FireBallObserver : public Observer
    {
    public:
        FireBallObserver(FireBallComponent* fireBallComponent, TileMap* tileMap):
            m_pFireBallComponent(fireBallComponent) , m_pTileMap(tileMap) {}

        void OnNotify(const GameObject& gameObject, Event event) override
        {
            if (event != EVENT_COLLISION)
                return;

            if (gameObject.HasComponent<EnemyComponent>() || gameObject.HasComponent<VersusEnemyComponent>())
            {
                auto enemy = gameObject.GetComponent<EnemyComponent>();
                auto playerEnemy = gameObject.GetComponent<VersusEnemyComponent>();
                if (enemy)
                {
                    std::cout << "enemy dies from fireball" << std::endl;
                    enemy->Die();
                    EventManager::GetInstance().FireEvent(EVENT_ENEMY_KILLED_BY_FIREBALL, nullptr);
                    m_pFireBallComponent->GetOwner()->MarkForDestruction();
                    return;
                }

                if (playerEnemy)
                {
                    std::cout << "player enemy dies from fireball" << std::endl;
                    playerEnemy->Die();
                   //EventManager::GetInstance().FireEvent(EVENT_ENEMY_KILLED_BY_FIREBALL, nullptr);
                    m_pFireBallComponent->GetOwner()->MarkForDestruction();
                    return;
                }
            }
        }

    private:
        FireBallComponent* m_pFireBallComponent;
        TileMap* m_pTileMap = nullptr;
    };


    FireBallComponent::FireBallComponent(GameObject* owner, Scene& scene, const glm::vec2& direction , TileMap* tileMap)
        : Component(owner), m_Scene(scene), m_Direction(direction) , m_pTileMap(tileMap)
    {
        m_pTransform = owner->AddComponent<Transform>();
        m_pAnimationComponent = owner->AddComponent<AnimationComponent>();
        owner->AddComponent<RenderComponent>("Fireball.png", 16, 16);
        m_pAnimationComponent->AddAnimation("Fire", "FireBall.png", 24/3, 8, 3, 0.1f);
        m_pAnimationComponent->PlayAnimation("Fire");

        m_pCollisionComponent = owner->AddComponent<CollisionComponent>(16.f, 16.f, &scene);
        m_pCollisionComponent->AddObserver(std::make_shared<FireBallObserver>(this , tileMap));
    }

    void FireBallComponent::Update(float deltaTime)
    {
        if (m_pTransform)
        {
            glm::vec3 pos = m_pTransform->GetLocalPosition();
            pos.x += m_Direction.x * m_Speed * deltaTime;
            pos.y += m_Direction.y * m_Speed * deltaTime;
            m_pTransform->SetLocalPosition(pos.x, pos.y, pos.z);

            int tileX = static_cast<int>(round(pos.x / TileMap::TILE_WIDTH));
            int tileY = static_cast<int>(round(pos.y / TileMap::TILE_HEIGHT));

            auto tileType = m_pTileMap->GetTile(tileX, tileY);

            if (tileType != TileType::Empty && tileType != TileType::Hole)
            {
                std::cout << "fireball hit dirt tile" << std::endl;
                GetOwner()->MarkForDestruction();
                //return;
            }
        }
    }
}
