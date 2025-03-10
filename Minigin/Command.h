#pragma once
#include "GameObject.h"

namespace dae
{
    class Command {
    public:
        virtual ~Command() = default;
        virtual void Execute(float deltaTime = 0.f) = 0;
    };

    class MoveUpCommand final : public Command {
    public:
        explicit MoveUpCommand(GameObject* pGameObject, float speed)
            : m_pGameObject(pGameObject), m_Speed(speed) {}

        void Execute(float deltaTime) override;

    private:
        GameObject* m_pGameObject;
        float m_Speed;
    };

    class MoveDownCommand final : public Command {
    public:
        explicit MoveDownCommand(GameObject* pGameObject, float speed)
            : m_pGameObject(pGameObject), m_Speed(speed) {}

        void Execute(float deltaTime) override;

    private:
        GameObject* m_pGameObject;
        float m_Speed;
    };

    class MoveLeftCommand final : public Command {
    public:
        explicit MoveLeftCommand(GameObject* pGameObject, float speed)
            : m_pGameObject(pGameObject), m_Speed(speed) {}

        void Execute(float deltaTime) override;

    private:
        GameObject* m_pGameObject;
        float m_Speed;
    };

    class MoveRightCommand final : public Command {
    public:
        explicit MoveRightCommand(GameObject* pGameObject, float speed)
            : m_pGameObject(pGameObject), m_Speed(speed) {}

        void Execute(float deltaTime) override;

    private:
        GameObject* m_pGameObject;
        float m_Speed;
    };
}

