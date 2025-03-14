#pragma once
#include "GameObject.h"
#include <vec2.hpp>

namespace dae
{
    class Command {
    public:
        virtual ~Command() = default;
        virtual void Execute(float deltaTime = 0.f) = 0;
    };

    class MoveCommand final : public Command {
    public:
        explicit MoveCommand(GameObject* pGameObject, float speed, glm::vec2 direction)
            : m_pGameObject(pGameObject), m_Speed(speed), m_Direction(direction) {}

        void Execute(float deltaTime) override;

    private:
        GameObject* m_pGameObject;
        float m_Speed;
        glm::vec2 m_Direction;  
    };
}

