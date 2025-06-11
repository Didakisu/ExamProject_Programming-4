#pragma once
#include <vector>
#include <memory>
#include "Scene.h"
#include "LevelLoader.h"
#include "TileMap.h"
#include "Observer.h"

namespace dae
{
    class EnemySpawner;

    constexpr dae::Event EVENT_ENEMY_DIED{ USER_SPACE + 2 };

    class EnemySpawnerObserver : public Observer
    {
    public:
        explicit EnemySpawnerObserver(EnemySpawner* enemySpawner);
        void OnNotify(const GameObject& gameObject, Event event) override;

    private:
        EnemySpawner* m_pEnemySpawner;

    };


    class EnemySpawner: public Component
    {
    public:
        EnemySpawner(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap, const std::vector<dae::SpawnPoint>& spawnPoints, int totalEnemies, float cooldownSeconds);

        void Update(float deltaTime) override;
        void OnEnemyDied();

    private:
        void SpawnNextEnemy(bool countSpawned);

        Scene& m_Scene;
        std::shared_ptr<TileMap> m_pTileMap;
        const std::vector<dae::SpawnPoint>& m_SpawnPoints;

        int m_TotalEnemies = 0;
        int m_SpawnedEnemies = 0;
        size_t m_NextSpawnIndex = 0;

        float m_CooldownSeconds = 2.f;
        float m_Timer = 0.f;

        int m_AliveEnemies = 0;

        const int m_MaxEnemiesAlive{4};
    };
}