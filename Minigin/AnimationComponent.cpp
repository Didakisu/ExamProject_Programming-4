#include "AnimationComponent.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include <string>
#include <unordered_map>


namespace dae
{
    AnimationComponent::AnimationComponent(GameObject* owner, int frameWidth, int frameHeight, int numFrames, float frameTime)
        : Component(owner),
        m_frameWidth(frameWidth),
        m_frameHeight(frameHeight),
        m_numFrames(numFrames),
        m_frameTime(frameTime),
        m_elapsedTime(0.f),
        m_currentFrame(0)
    {
    }

    void AnimationComponent::Update(float deltaTime)
    {
        m_elapsedTime += deltaTime;
        if (m_elapsedTime >= m_frameTime)
        {
            m_elapsedTime -= m_frameTime;
            m_currentFrame = (m_currentFrame + 1) % m_numFrames;
        }

        // Tell RenderComponent which frame to use
        auto* pRenderComp = m_pOwner->GetComponent<RenderComponent>();
        if (pRenderComp)
        {
            SDL_Rect frameRect{};
            frameRect.x = m_currentFrame * m_frameWidth;
            frameRect.y = 0;
            frameRect.w = m_frameWidth;
            frameRect.h = m_frameHeight;

            pRenderComp->SetSourceRect(frameRect);
        }
    }


    void AnimationComponent::Render() const
    {
       
    }

   

}
