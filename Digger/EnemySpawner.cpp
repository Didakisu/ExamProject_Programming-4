#include "EnemySpawner.h"
#include "LevelLoader.h"
#include <iostream>
#include "EnemyComponent.h"
#include "CherryPowerUp.h"

namespace dae
{

    EnemySpawnerObserver::EnemySpawnerObserver(EnemySpawner* enemySpawner)
        : m_pEnemySpawner(enemySpawner)
    {
    }

    void EnemySpawnerObserver::OnNotify(const GameObject& gameObject, Event event)
    {
        if (event != EVENT_ENEMY_DIED)
            return;

        std::cout << "EnemySpawnerObserver received enemy died event." << std::endl;

        if (!gameObject.HasComponent<EnemyComponent>())
        {
            std::cout << "Not an enemy game object, ignoring." << std::endl;
            return;
        }

        if (m_pEnemySpawner)
        {
            m_pEnemySpawner->OnEnemyDied();
        }
    }

    EnemySpawner::EnemySpawner(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap,
        const std::vector<dae::SpawnPoint>& spawnPoints, int totalEnemies, float cooldownSeconds)
        : Component(owner)
        , m_Scene(scene)
        , m_pTileMap(tileMap)
        , m_SpawnPoints(spawnPoints)
        , m_TotalEnemies(totalEnemies)
        , m_CooldownSeconds(cooldownSeconds)
        , m_Timer(0.f)
        , m_SpawnedEnemies(0)
        , m_AliveEnemies(0)
        , m_NextSpawnIndex(0)
    {
        if (!m_SpawnPoints.empty() && m_TotalEnemies > 0)
        {
            SpawnNextEnemy(true);
        }
    }

    void EnemySpawner::Update(float deltaTime)
    {
        if (m_SpawnedEnemies >= m_TotalEnemies)
            return;

        m_Timer += deltaTime;

        if (m_Timer >= m_CooldownSeconds)
        {
            SpawnNextEnemy(true);
            m_Timer = 0.f;
        }
    }

    void EnemySpawner::SpawnNextEnemy(bool countSpawned)
    {
        if (m_SpawnPoints.empty())
        {
            std::cerr << "Spawn points empty, cannot spawn enemy." << std::endl;
            return;
        }

        if (m_AliveEnemies >= m_TotalEnemies)
            return;

        if (countSpawned && m_SpawnedEnemies >= m_TotalEnemies)
            return;

        const auto& pos = m_SpawnPoints[m_NextSpawnIndex % m_SpawnPoints.size()];

        m_NextSpawnIndex++;

        if (countSpawned)
            m_SpawnedEnemies++;

        m_AliveEnemies++;

        LevelLoader::SpawnEnemy(m_Scene, pos.x, pos.y, pos.z, m_pTileMap, this);

        if (m_SpawnedEnemies >= m_TotalEnemies)
        {
            //spawn cherries here(when spawned enemies are actually the number of the total enemies)

            /*auto cherry = std::make_shared<GameObject>();
            cherry->AddComponent<RenderComponent>("cherries.png", 32, 32);
            cherry->AddComponent<Transform>()->SetLocalPosition(TileMap::TILE_WIDTH * 14.f, TileMap::TILE_HEIGHT * 2.f, 3.f);
            auto collision = cherry->AddComponent<CollisionComponent>(32.f, 32.f, &scene);
            auto cherryObserver = std::make_shared<CherryPowerUp>(cherry.get());
            collision->AddObserver(cherryObserver);
            scene.Add(cherry);*/
        }
    }

    void EnemySpawner::OnEnemyDied()
    {
        std::cout << "EnemySpawner notified of enemy death." << std::endl;

        if (m_AliveEnemies > 0)
            --m_AliveEnemies;

        SpawnNextEnemy(false);
    }
}