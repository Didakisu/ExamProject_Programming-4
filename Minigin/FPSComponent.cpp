#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

namespace dae
{
    FPSComponent::FPSComponent(std::shared_ptr<GameObject> owner)
        : Component(owner), m_frameCount(0), m_fps(0), m_lastTime(std::chrono::high_resolution_clock::now())
    {}

    void FPSComponent::Update()
    {
        m_frameCount++;  

        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - m_lastTime;

        if (elapsed.count() >= 0.5f)
        {
            m_fps = m_frameCount;  
            m_frameCount = 0;  
            m_lastTime = now;  

            auto* textComponent = m_pOwner->GetComponent<TextComponent>();
            if (textComponent)
            {
                textComponent->SetText("FPS: " + std::to_string(m_fps));
            }
        }
    }
}
