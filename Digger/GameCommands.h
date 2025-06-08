#pragma once
#include "Command.h"
#include "TileMap.h"
#include "Scene.h"
#include "GameStates.h"


namespace dae { class GameController; }
//namespace dae { class EndScreenState; }

class MoveCommand final : public dae::Command {
public:
    explicit MoveCommand(dae::GameObject* pGameObject
        , float speed
        , glm::vec2 direction
        , std::shared_ptr<TileMap> tileMap
        , dae::Scene& scene)
        : m_pGameObject(pGameObject)
        , m_Speed(speed)
        , m_Direction(direction)
        , m_pTileMap(tileMap)
        , m_Scene(scene)
    {
    }

    void Execute(float deltaTime) override;
    bool IsDiggable(TileType tile);

private:
    dae::GameObject* m_pGameObject;
    float m_Speed;
    glm::vec2 m_Direction;
    std::shared_ptr<TileMap> m_pTileMap;
    dae::Scene& m_Scene;
};

//class KillPlayerCommand final: public dae::Command
//{
//public:
//    explicit KillPlayerCommand(dae::GameObject* gameObject)
//        : m_GameObject(gameObject) 
//    {
//    }
//
//    void Execute(float deltaTime) override;
//private:
//    dae::GameObject* m_GameObject;
//};

class CollectCommand final : public dae::Command
{
public:
    explicit CollectCommand(dae::GameObject* gameObject, std::vector<dae::GameObject*> collectibles):
        m_GameObject(gameObject), m_Collectibles(collectibles) 
    {

    }
    void Execute(float deltaTime) override;
private:
    dae::GameObject* m_GameObject;
    std::vector<dae::GameObject*> m_Collectibles;
};

class FireCommand final : public dae::Command
{
public:
    FireCommand(dae::GameObject* gameObject, dae::Scene& scene) :
        m_GameObject(gameObject), m_Scene(scene)
    {

    }
    void Execute(float deltaTime) override;
private:
    dae::GameObject* m_GameObject{};
    dae::Scene& m_Scene;
};


class RequestStateChangeCommand : public dae::Command
{
public:
    RequestStateChangeCommand(dae::GameController* controller, const std::string& targetState)
        : m_pController(controller), m_TargetState(targetState), m_IsRequested(false) 
    {
    }
    void Execute(float) override;
    bool IsRequested() const { return m_IsRequested; }
    void Reset() { m_IsRequested = false; }

private:
    dae::GameController* m_pController;
    std::string m_TargetState;
    bool m_IsRequested;
};


//class ChangeInitialLetterCommand : public dae::Command
//{
//public:
//    ChangeInitialLetterCommand(dae::GameController* controller, int delta)
//        : m_Controller(controller), m_Delta(delta) {
//    }
//    void Execute(float) override;
//private:
//    dae::GameController* m_Controller;
//    int m_Delta;
//};