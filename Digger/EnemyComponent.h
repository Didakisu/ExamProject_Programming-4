#pragma once
#include "EnemyComponent.h"
#include "Component.h"

namespace dae
{
    class EnemyComponent final : public Component
    {
        explicit EnemyComponent(GameObject* owner);
        void Update(float deltaTime) override;
        void FixedUpdate() override;
        void Render() const override;
    };
}