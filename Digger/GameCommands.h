#pragma once
#include "Command.h"
#include "TileMap.h"
#include "Scene.h"
#include <SDLMixerSoundSystem.h>
#include <StateMachine.h>
#include <StateMachine.h>

namespace dae { class GameController; }
namespace dae { class RegularGameplayMode; }


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

    void SetRestrictMovement(bool enabled) { m_RestrictedMovement = enabled; }
private:
    dae::GameObject* m_pGameObject;
    float m_Speed;
    glm::vec2 m_Direction;
    std::shared_ptr<TileMap> m_pTileMap;
    dae::Scene& m_Scene;

    bool m_RestrictedMovement{false};
};

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


class ChangeInitialLetterCommand : public dae::Command
{
public:
    ChangeInitialLetterCommand(dae::GameController* controller, int delta)
        : m_Controller(controller), m_Delta(delta) {
    }
    void Execute(float) override;
private:
    dae::GameController* m_Controller;
    int m_Delta;
};


class ConfirmInitialLetterCommand final : public dae::Command
{
public:
    explicit ConfirmInitialLetterCommand(dae::GameController* controller)
        : m_Controller(controller) {
    }
    void Execute(float) override;
private:
    dae::GameController* m_Controller;
};


class UndoInitialCommand : public dae::Command
{
public:
    UndoInitialCommand(dae::GameController* controller) : m_Controller(controller) {}
    void Execute(float) override;
private:
    dae::GameController* m_Controller{};
};


class MainMenuSelectCommand : public dae::Command
{
public:
    MainMenuSelectCommand(dae::GameController* controller, int delta) : m_Controller(controller), m_Delta(delta) {}
    void Execute(float) override;
private:
    dae::GameController* m_Controller;
    int m_Delta;
};


class MainMenuConfirmCommand : public dae::Command
{
public:
    MainMenuConfirmCommand(dae::GameController* controller) : m_Controller(controller) {}
    void Execute(float) override;
private:
    dae::GameController* m_Controller;
};


class ToggleMuteCommand : public dae::Command 
{
public:
    ToggleMuteCommand(dae::SoundSystem& soundSystem) : m_SoundSystem(soundSystem) {}
    void Execute(float) override;
private:
    dae::SoundSystem& m_SoundSystem;
};


class SkipLevelCommand final : public dae::Command
{
public:
    explicit SkipLevelCommand(dae::GameController* controller) : m_Controller(controller) {}
    void Execute(float) override;
private:
    dae::GameController* m_Controller;
};