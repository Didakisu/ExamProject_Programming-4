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
#include "GoldBagComponent.h"
#include "GoldBagStates.h"

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

    const float zDirt = 0.f;
    const float zGem = 1.f;
    const float zGoldBag = 3.f;

    for (size_t y = 0; y < lines.size(); ++y)
    {
        for (size_t x = 0; x < lines[y].length(); ++x)
        {
            float fxDirt = static_cast<float>(x * TileMap::TILE_WIDTH);
            float fyDirt = static_cast<float>(y * TileMap::TILE_HEIGHT);

            TileType type = outMap.GetTile(static_cast<int>(x), static_cast<int>(y));

            switch (type)
            {
            case TileType::Empty:
                SpawnEmpty(scene, fxDirt, fyDirt, zGem, std::nullopt);
                break;
            case TileType::Dirt:
                SpawnDirt(scene, fxDirt, fyDirt, zDirt);
                break;
            case TileType::Gem:
                SpawnDirt(scene, fxDirt, fyDirt, zDirt);
                SpawnGem(scene, fxDirt, fyDirt, zGem);
                break;
            case TileType::GoldBag:
                SpawnDirt(scene, fxDirt, fyDirt, zDirt);
                SpawnGoldBag(scene, fxDirt, fyDirt, zGoldBag, &outMap);
                break;
            case TileType::Hole:
                SpawnCornerHole(scene, fxDirt, fyDirt, zGem);
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
    
    go->AddComponent<RenderComponent>("reworkedHole.png", TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
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

void dae::LevelLoader::SpawnUI(Scene& scene, float x, float y, float z)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>("Rectangle.png", TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    scene.Add(go);
}

void dae::LevelLoader::SpawnCornerHole(Scene& scene, float x, float y, float z)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>("hole_with_background.png", TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    scene.Add(go);
}

void dae::LevelLoader::SpawnGem(Scene& scene, float x, float y, float z)
{
    auto pEmeraldCollectible = std::make_shared<GameObject>();
    pEmeraldCollectible->AddComponent<RenderComponent>("Gem.png", 25, 20 );
    pEmeraldCollectible->AddComponent<Transform>()->SetLocalPosition(x, y, z);

    std::shared_ptr<dae::Observer>collectible = std::make_shared<dae::CollectibleComponent>(pEmeraldCollectible.get());
    auto collision = pEmeraldCollectible->AddComponent<dae::CollisionComponent>(30.f, 32.f, &scene);
    collision->AddObserver(collectible);

    scene.Add(pEmeraldCollectible);
}

void dae::LevelLoader::SpawnGoldBag(Scene& scene, float x, float y, float z, TileMap* pTileMap)
{
    auto pGoldBag = std::make_shared<dae::GameObject>();

    pGoldBag->AddComponent<dae::RenderComponent>("CoinBagSingle.png", 32, 32);
    pGoldBag->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    auto collision = pGoldBag->AddComponent<dae::CollisionComponent>(30.f, 30.f, &scene);
    pGoldBag->AddComponent<dae::AnimationComponent>();

    auto goldBagLogic = pGoldBag->AddComponent<dae::GoldBagComponent>();
    goldBagLogic->SetTileMap(pTileMap); 
    goldBagLogic->SetScene(&scene);//set scene here, i need it in goldbag collectible state

    collision->AddObserver(std::make_shared<GoldBagObserver>(goldBagLogic));

    scene.Add(pGoldBag);
}