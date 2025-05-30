#pragma once
#include "Scene.h"
#include <string>
#include "DirectionComponent.h"
#include <optional>

class TileMap;

namespace dae {

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
    };

}