#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

namespace dae
{
    FPSComponent::FPSComponent(GameObject* owner)
        : Component(owner), m_frameCount(0), m_fps(0), m_lastTime(std::chrono::high_resolution_clock::now())
    {
        m_textComponent = owner->GetComponent<TextComponent>();
    }

    void FPSComponent::Update(float /*deltaTime*/)
    {
        m_frameCount++;

        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - m_lastTime;

        if (elapsed.count() >= 1.f)
        {
            m_fps = m_frameCount;
            m_frameCount = 0;
            m_lastTime = now;

            if (m_textComponent)
            {
                m_textComponent->SetText("FPS: " + std::to_string(m_fps));
            }
        }
    }
}

