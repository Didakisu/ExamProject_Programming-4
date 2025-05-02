#pragma once
#include "Command.h"

class MoveCommand final : public dae::Command {
public:
    explicit MoveCommand(dae::GameObject* pGameObject, float speed, glm::vec2 direction)
        : m_pGameObject(pGameObject), m_Speed(speed), m_Direction(direction) {
    }

    void Execute(float deltaTime) override;

private:
    dae::GameObject* m_pGameObject;
    float m_Speed;
    glm::vec2 m_Direction;
};  

class KillPlayerCommand final: public dae::Command
{
public:
    explicit KillPlayerCommand(dae::GameObject* gameObject)
        : m_GameObject(gameObject) {
    }

    void Execute(float deltaTime) override;
private:
    dae::GameObject* m_GameObject;
};

//class CollectCommand final: public dae::Command
//{
//public:
//    explicit CollectCommand(dae::GameObject* gameObject)
//        : m_GameObject(gameObject) {
//    }
//    void Execute(float deltaTime) override;
//private:
//    dae::GameObject* m_GameObject;
//};

class CollectCommand final : public dae::Command
{
public:
    explicit CollectCommand(dae::GameObject* gameObject, std::vector<dae::GameObject*> collectibles)
        : m_GameObject(gameObject), m_Collectibles(collectibles) {
    }

    void Execute(float deltaTime) override;

private:
    dae::GameObject* m_GameObject;
    std::vector<dae::GameObject*> m_Collectibles;
};





