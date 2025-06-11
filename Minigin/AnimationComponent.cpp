#include "AnimationComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include <string>
#include <unordered_map>
#include <SDL.h>
#include <ostream>
#include <iostream>

namespace dae
{

    AnimationComponent::AnimationComponent(GameObject* owner)
        : Component(owner),
        m_elapsedTime(0.f),
        m_currentFrame(0),
        m_currentAnimation("") 
    {
    }

    void AnimationComponent::AddAnimation(const std::string& name, const std::string& spriteSheet, int frameWidth, int frameHeight, int numFrames, float frameTime, bool useDirection , bool loop )
    {
        dae::Animation anim = { spriteSheet, frameWidth, frameHeight, numFrames, frameTime , useDirection , loop };
        m_animations[name] = anim;
    }

    void AnimationComponent::PlayAnimation(const std::string& name)
    {
        if (m_currentAnimation != name && m_animations.find(name)!= m_animations.end())
        {
            auto* pRenderComp = m_pOwner->GetComponent<RenderComponent>();
            if (pRenderComp && m_animations[name].spriteSheet != "")
            {
                pRenderComp->SetTexture(m_animations[name].spriteSheet);
            }
            m_currentAnimation = name;
            m_currentFrame = 0;  
            m_elapsedTime = 0.f; 
        }
    }

    void AnimationComponent::Update(float deltaTime)
    {
        if (m_currentAnimation.empty()) return;

        if (m_animations.find(m_currentAnimation) == m_animations.end())
        {
            return; 
        }

        const dae::Animation& currentAnim = m_animations[m_currentAnimation];

        m_elapsedTime += deltaTime;

        if (m_elapsedTime >= currentAnim.frameTime)
        {
            m_elapsedTime -= currentAnim.frameTime;

            if (m_currentFrame + 1 < currentAnim.numFrames)
            {
                m_currentFrame++;
            }
            else if (currentAnim.loop)
            {
                m_currentFrame = 0;
            }
        }


        auto* pRenderComp = m_pOwner->GetComponent<RenderComponent>();
        if (pRenderComp)
        {
            SDL_Rect frameRect{};
            frameRect.x = m_currentFrame * currentAnim.frameWidth;
            frameRect.y = 0;  
            frameRect.w = currentAnim.frameWidth;
            frameRect.h = currentAnim.frameHeight;

            pRenderComp->SetSourceRect(frameRect);
        }
    }

    void AnimationComponent::Render() const
    {

    }

    void AnimationComponent::StopAnimation()
    {
        m_currentAnimation = "";
        m_currentFrame = 0;
        m_elapsedTime = 0.f;

        auto* pRenderComp = m_pOwner->GetComponent<RenderComponent>();
        if (pRenderComp)
        {
            SDL_Rect frameRect{};

            if (!m_animations.empty())
            {
                const auto& firstAnim = m_animations.begin()->second;
                frameRect.x = 0;
                frameRect.y = 0;
                frameRect.w = firstAnim.frameWidth;
                frameRect.h = firstAnim.frameHeight;
            }
            else
            {
                frameRect.w = 0;
                frameRect.h = 0;
            }

            pRenderComp->SetSourceRect(frameRect);
        }
    }

    const std::string& AnimationComponent::GetCurrentAnimationName() const
    {
        return m_currentAnimation; 
    }

    bool AnimationComponent::CurrentAnimationUsesDirection() const
    {
        auto it = m_animations.find(m_currentAnimation);
        if (it == m_animations.end())
        {
            return true;
        }

        return it->second.useDirection;
    }

} 
