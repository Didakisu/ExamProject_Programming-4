#pragma once
#include "Component.h"
#include "TextComponent.h"
#include <chrono>
#include <string>

namespace dae
{
    class FPSComponent final : public Component
    {
    public:
        explicit FPSComponent(GameObject* owner); 

        void Update(float deltaTime) override;

    private:
        int m_frameCount;
        int m_fps;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTime;
        TextComponent* m_textComponent;
    };
}

