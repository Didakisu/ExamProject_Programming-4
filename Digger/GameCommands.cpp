#include "GameCommands.h"
#include "ScoreComponent.h"
#include "HealthComponent.h"
#include "EventManager.h"
#include "CollectibleComponent.h"
#include "SceneManager.h"
#include "Data.h"
#include <RenderComponent.h>
#include "DirectionComponent.h" 
#include "TileMap.h"
#include "LevelLoader.h"
#include <unordered_set>
#include <Physics.h>


constexpr int DISTANCE_EPSILON = 10;

void MoveCommand::Execute(float deltaTime)
{

    if (!m_pGameObject || !m_pGameObject->GetTransform() || !m_pTileMap) return;

    glm::vec2 dir = m_Direction;
    //prevent diagonal movement
    if (dir.x != 0.0f && dir.y != 0.0f)
        dir.y = 0.0f;

    //move character
    auto transform = m_pGameObject->GetTransform();
    glm::vec3 pos = transform->GetLocalPosition();


    int tx = static_cast<int>(round(pos.x / TileMap::TILE_WIDTH));
    int ty = static_cast<int>(round(pos.y / TileMap::TILE_HEIGHT));

    auto oldDir = m_pGameObject->GetComponent<dae::DirectionComponent>()->GetDirectionVector();
    auto oldDirEnum = m_pGameObject->GetComponent<dae::DirectionComponent>()->GetDirection();

    TileType tile = m_pTileMap->GetTile(tx, ty);

    if (oldDir != dir)
    {
        int desiredXPos = tx * TileMap::TILE_WIDTH;
        int desiredYPos = ty * TileMap::TILE_HEIGHT;

        int distanceX = abs(desiredXPos - static_cast<int>(pos.x));
        int distanceY = abs(desiredYPos - static_cast<int>(pos.y));

        bool snapped = false;

        switch (oldDirEnum)
        {
        case dae::Direction::Left:
        case dae::Direction::Right:
            if (distanceX > DISTANCE_EPSILON)
            {
                dir = oldDir;
            }
            else
            {
                pos.x = static_cast<float>(desiredXPos);
                snapped = true;
            }
            break;
        case dae::Direction::Down:
        case dae::Direction::Up:
            if (distanceY > DISTANCE_EPSILON)
            {
                dir = oldDir;
            }
            else
            {
                pos.y = static_cast<float>(desiredYPos);
                snapped = true;
            }
            break;
        }

        if (snapped)
        {
            if (tile == TileType::Empty)
            {
                std::cout << "tile is empty" << std::endl;
            }
            float fxCorner = static_cast<float>(tx * TileMap::TILE_WIDTH_EMPTY);
            float fyCorner = static_cast<float>(ty * TileMap::TILE_HEIGHT_EMPTY);
            const float z = 1.f;
            dae::LevelLoader::SpawnCornerHole(m_Scene, fxCorner, fyCorner, z);
        }
    }

    //check for crossing paths , horizontal and vertical
    bool up = m_pTileMap->GetTile(tx, ty - 1) == TileType::Empty;
    bool down = m_pTileMap->GetTile(tx, ty + 1) == TileType::Empty;
    bool left = m_pTileMap->GetTile(tx - 1, ty) == TileType::Empty;
    bool right = m_pTileMap->GetTile(tx + 1, ty) == TileType::Empty;

    //if crossing horizontally and vertically, spawn corner hole
    bool isCrossing = (left || right) && (up || down);

    if (isCrossing)
    {
        float fxCorner = static_cast<float>(tx * TileMap::TILE_WIDTH_EMPTY);
        float fyCorner = static_cast<float>(ty * TileMap::TILE_HEIGHT_EMPTY);
        const float z = 1.f;
        dae::LevelLoader::SpawnCornerHole(m_Scene, fxCorner, fyCorner, z);
    }

    glm::vec2 nextPos = {
        pos.x + dir.x * m_Speed * deltaTime,
        pos.y + dir.y * m_Speed * deltaTime
    };

    const float playerWidth = static_cast<float>(TileMap::TILE_WIDTH);
    const float playerHeight = static_cast<float>(TileMap::TILE_HEIGHT);

    const float mapWidthPixels = static_cast<float>(m_pTileMap->GetWidth() * TileMap::TILE_WIDTH);
    const float mapHeightPixels = static_cast<float>((m_pTileMap->GetHeight() * TileMap::TILE_HEIGHT) - TileMap::TILE_HEIGHT); //-32, becasue of the bottom

    if (nextPos.x < 0.f || nextPos.y < 0.f ||
        (nextPos.x + playerWidth) > mapWidthPixels ||
        (nextPos.y + playerHeight) > mapHeightPixels)
    {
        return;
    }

    if (dae::Physics::CanMoveTo(m_pGameObject, nextPos, &m_Scene))
    {
        transform->SetLocalPosition(nextPos.x, nextPos.y, pos.z);
    }

    //update facing direction
    if (auto dc = m_pGameObject->GetComponent<dae::DirectionComponent>())
    {
        if (dir.x > 0) dc->SetDirection(dae::Direction::Right);
        else if (dir.x < 0) dc->SetDirection(dae::Direction::Left);
        else if (dir.y < 0) dc->SetDirection(dae::Direction::Up);
        else if (dir.y > 0) dc->SetDirection(dae::Direction::Down);
    }

    int desiredXPos = tx * TileMap::TILE_WIDTH;
    int desiredYPos = ty * TileMap::TILE_HEIGHT;

    int distanceX = abs(desiredXPos - static_cast<int>(pos.x));
    int distanceY = abs(desiredYPos - static_cast<int>(pos.y));

    if (distanceX < TileMap::TILE_WIDTH * 0.15 && distanceY < TileMap::TILE_HEIGHT * 0.15)
    {
        if (tile == TileType::Dirt || tile == TileType::Gem || tile == TileType::GoldBag)
        {
            m_pTileMap->SetTile(tx, ty, TileType::Empty);

            float fxEmpty = static_cast<float>(tx * TileMap::TILE_WIDTH_EMPTY);
            float fyEmpty = static_cast<float>(ty * TileMap::TILE_HEIGHT_EMPTY);
            const float zEmpty = 1.f;

            dae::Direction currentDir = m_pGameObject->GetComponent<dae::DirectionComponent>()->GetDirection();
            dae::LevelLoader::SpawnEmpty(m_Scene, fxEmpty, fyEmpty, zEmpty, currentDir);
        }
    }
}

void KillPlayerCommand::Execute(float /*deltaTime*/)
{
    if (m_GameObject)
    {
        auto healthComp = m_GameObject->GetComponent<dae::HealthComponent>();
        if (healthComp) {
            healthComp->LoseLife();
        }
    }
}

void CollectCommand::Execute(float /*deltaTime*/)
{

    //if (m_GameObject)
    //{
    //   /* auto scoreComp = m_GameObject->GetComponent<dae::ScoreComponent>();
    //    if (scoreComp)
    //    {
    //        scoreComp->PickUpCoin();
    //    }*/
    //    
    //    auto& eventManager = dae::EventManager::GetInstance();
    //    eventManager.FireEvent(EVENT_PLAYER_COLLECT_ITEM, nullptr , m_GameObject);
    //}


    if (!m_GameObject) return;

    for (auto collectibleObj : m_Collectibles)
    {
        if (auto collectible = collectibleObj->GetComponent<dae::CollectibleComponent>())
        {
            if (!collectible->IsCollected())
            {
                auto playerPos = m_GameObject->GetComponent<dae::Transform>()->GetWorldPosition();
                auto objPos = collectibleObj->GetComponent<dae::Transform>()->GetWorldPosition();

                float dist = glm::distance(glm::vec2(playerPos.x, playerPos.y), glm::vec2(objPos.x, objPos.y));
                if (dist < 50.f)
                {
                    collectible->OnCollected();
                    break;
                }
            }
        }
    }

}