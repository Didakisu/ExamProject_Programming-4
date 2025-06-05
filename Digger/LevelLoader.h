#pragma once
#include "Scene.h"
#include <string>
#include "DirectionComponent.h"
#include <optional>
#include <vector>

class TileMap;


namespace dae 
{
    class EnemySpawner;

    struct SpawnPoint {
        float x, y, z;
    };

    class LevelLoader 
    {
    public:
        static void LoadLevel(const std::string& filename, Scene& scene, TileMap& outMap);
        static void SpawnDirt(Scene& scene, float x, float y, float z);
        static void SpawnEmpty(Scene& scene, float x, float y, float z, std::optional<Direction> dirOpt);
        static void SpawnCornerHole(Scene& scene, float x, float y, float z);
        static void SpawnGem(Scene& scene, float x, float y, float z);
        static void SpawnGoldBag(Scene& scene, float x, float y, float z, TileMap* pTileMap);
        static void SpawnUI(Scene& scene, float x, float y, float z);
        static void SpawnEnemy(Scene& scene, float x, float y, float z, std::shared_ptr<TileMap> pTileMap, EnemySpawner* pSpawner);
        //
        const std::vector<SpawnPoint>& GetEnemySpawnPositions() const { return m_EnemySpawnPositions; }

    private:
       static std::vector<SpawnPoint> m_EnemySpawnPositions;
    };

}