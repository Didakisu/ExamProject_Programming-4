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
#include "EnemyComponent.h"
#include "EnemySpawner.h"
#include "Data.h"

std::vector<dae::SpawnPoint> dae::LevelLoader::m_EnemySpawnPositions{};
std::vector<std::shared_ptr<dae::Observer>> dae::LevelLoader::m_CollectibleObservers{};
int dae::LevelLoader::m_TotalGems = 0;

std::vector<dae::GameObject*> dae::LevelLoader::m_DirtTiles{};
std::string dae::LevelLoader::m_CurrentDirtTexture{};
std::string dae::LevelLoader::m_OriginalDirtTexture{};

void dae::LevelLoader::LoadLevel(const std::string& filename, Scene& scene, TileMap& outMap, const std::string& dirtTexture)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << filename << "\n";
        return;
    }

    m_EnemySpawnPositions.clear();
    m_CollectibleObservers.clear();
    m_TotalGems = 0;

    //
    m_DirtTiles.clear();
    m_OriginalDirtTexture.clear();

    //
    m_CurrentDirtTexture = dirtTexture;
    if (m_OriginalDirtTexture.empty())
    {
        m_OriginalDirtTexture = dirtTexture;
    }

    //
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);

    outMap.LoadFromText(lines);

    const float zDirt = 0.f;
    const float zGem = 1.f;
    const float zGoldBag = 3.f;

    for (int y = 0; y < lines.size(); ++y)
    {
        for (int x = 0; x < lines[y].length(); ++x)
        {
            float fxDirt = static_cast<float>(x * TileMap::TILE_WIDTH);
            float fyDirt = static_cast<float>(y * TileMap::TILE_HEIGHT);

            TileType type = outMap.GetTile(static_cast<int>(x), static_cast<int>(y));

            switch (type)
            {
            case TileType::Empty:
                SpawnDirtBackground(scene, fxDirt, fyDirt, zDirt, dirtTexture);
                SpawnEmpty(scene, fxDirt, fyDirt, zGem, std::nullopt);
                break;
            case TileType::Dirt:
                SpawnDirt(scene, fxDirt, fyDirt, zDirt, dirtTexture);
                break;
            case TileType::Gem:
                SpawnDirt(scene, fxDirt, fyDirt, zDirt, dirtTexture);
                outMap.SetTile(x, y, TileType::Dirt);
                SpawnGem(scene, fxDirt, fyDirt, 4.f);
                ++m_TotalGems;
                break;
            case TileType::GoldBag:
                SpawnDirt(scene, fxDirt, fyDirt, zDirt, dirtTexture);
                outMap.SetTile(x, y, TileType::Dirt);
                SpawnGoldBag(scene, fxDirt, fyDirt, zGoldBag, &outMap);
                break;
            case TileType::Hole:
                SpawnDirtBackground(scene, fxDirt, fyDirt, zDirt, dirtTexture);
                SpawnCornerHole(scene, fxDirt, fyDirt, zGem);
                break;
            case TileType::Enemy:
                SpawnEmpty(scene, fxDirt, fyDirt, zGem, std::nullopt);
                outMap.SetTile(x, y, TileType::Empty);
                m_EnemySpawnPositions.push_back({ fxDirt, fyDirt, zGoldBag });
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
    
    go->AddComponent<RenderComponent>("holeCracks_01.png", TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    if (dirOpt.has_value())
        go->AddComponent<DirectionComponent>()->SetDirection(dirOpt.value());
    scene.Add(go);
}

void dae::LevelLoader::SpawnDirt(Scene& scene, float x, float y, float z, const std::string& texture)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>(texture, TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    scene.Add(go);
    m_DirtTiles.push_back(go.get());
}

void dae::LevelLoader::SpawnUI(Scene& scene, float x, float y, float z)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>("Rectangle.png", TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    scene.Add(go);
}

void dae::LevelLoader::SpawnEnemy(Scene& scene, float x, float y, float z, std::shared_ptr<TileMap> pTileMap, EnemySpawner* pSpawner)
{
    auto enemyGO = std::make_shared<GameObject>();
    enemyGO->AddComponent<Transform>()->SetLocalPosition(x , y , z);
    auto enemyComp = enemyGO->AddComponent<EnemyComponent>(scene, pTileMap);
    enemyComp->Initialize({ x, y, z } , pSpawner);
    scene.Add(enemyGO);
    std::cout << "Enemy spawned and initialized with observer." << std::endl;
}

void dae::LevelLoader::SpawnCornerHole(Scene& scene, float x, float y, float z)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>("reworkedHole_02.png", TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    scene.Add(go);
}

void dae::LevelLoader::SpawnGem(Scene& scene, float x, float y, float z)
{
    auto pEmeraldCollectible = std::make_shared<GameObject>();
    pEmeraldCollectible->AddComponent<RenderComponent>("Gem.png", 25, 20);
    pEmeraldCollectible->AddComponent<Transform>()->SetLocalPosition(x, y, z);

    auto collision = pEmeraldCollectible->AddComponent<dae::CollisionComponent>(20.f, 22.f, &scene);
    auto collectibleObserver = std::make_shared<dae::CollectibleComponent>(pEmeraldCollectible.get() ,EVENT_COLLECTED_GEM);
    collision->AddObserver(collectibleObserver);

    m_CollectibleObservers.push_back(collectibleObserver);

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

void dae::LevelLoader::SpawnDirtBackground(Scene& scene, float x, float y, float z, const std::string& texture)
{
    auto go = std::make_shared<GameObject>();
    go->AddComponent<RenderComponent>(texture, TileMap::TILE_WIDTH, TileMap::TILE_HEIGHT);
    go->AddComponent<Transform>()->SetLocalPosition(x, y, z);
    scene.Add(go);
    m_DirtTiles.push_back(go.get());
}

const std::string& dae::LevelLoader::GetCurrentDirtTexture()
{
    return m_CurrentDirtTexture;
}

void dae::LevelLoader::SetDirtTileTextures(const std::string& texturePath)
{
    for (auto* go : m_DirtTiles)
    {
        if (go)
        {
            auto render = go->GetComponent<RenderComponent>();
            if (render)
                render->SetTexture(texturePath);
        }
    }
    m_CurrentDirtTexture = texturePath;
}

const std::string& dae::LevelLoader::GetOriginalDirtTexture()
{
    return m_OriginalDirtTexture;
}



