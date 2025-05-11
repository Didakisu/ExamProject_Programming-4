#include "LevelLoader.h"
#include "TileMap.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "CollisionComponent.h"
#include "CollectibleComponent.h"
#include <fstream>
#include <sstream>
#include <DirectionComponent.h>
#include <AnimationComponent.h>
#include "ScoreComponent.h"
#include "HealthComponent.h"

void dae::LevelLoader::LoadLevel(const std::string& filename, Scene& scene, TileMap& outMap)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << filename << "\n";
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);

    outMap.LoadFromText(lines);

    const float zEmpty = -1.f;
    const float zDirt = 0.f;

    for (size_t y = 0; y < lines.size(); ++y)
    {
        for (size_t x = 0; x < lines[y].length(); ++x)
        {
            float fxDirt = static_cast<float>(x * TileMap::TILE_WIDTH);
            float fyDirt = static_cast<float>(y * TileMap::TILE_HEIGHT);

            float fxEmpty = static_cast<float>(x * TileMap::TILE_WIDTH_EMPTY);
            float fyEmpty = static_cast<float>(y * TileMap::TILE_HEIGHT_EMPTY);

            TileType type = outMap.GetTile(static_cast<int>(x), static_cast<int>(y));

            switch (type)
            {
            case TileType::Empty:
                SpawnEmpty(scene, fxEmpty, fyEmpty, zEmpty, std::nullopt);
                break;

            case TileType::Dirt:
                SpawnDirt(scene, fxDirt, fyDirt, zDirt);
                break;

            default:
                break;
            }
        }
    }
}


void dae::LevelLoader::SpawnEmpty(Scene& scene, float x, float y, float z, std::optional<Direction> dirOpt)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>("holeCracks_01.png", TileMap::TILE_WIDTH_EMPTY, TileMap::TILE_HEIGHT_EMPTY);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    if (dirOpt.has_value())
        go->AddComponent<DirectionComponent>()->SetDirection(dirOpt.value());
    scene.Add(go);
}

void dae::LevelLoader::SpawnDirt(Scene& scene, float x, float y, float z)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>("Backgrounds_1.png", TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    scene.Add(go);
}

void dae::LevelLoader::SpawnCornerHole(Scene& scene, float x, float y, float z)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>("hole.png", TileMap::TILE_WIDTH_EMPTY , TileMap::TILE_HEIGHT_EMPTY );
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    scene.Add(go);
}

