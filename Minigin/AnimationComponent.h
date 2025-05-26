#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>

namespace dae
{
    struct Animation
    {
        std::string spriteSheet;
        int frameWidth;
        int frameHeight;
        int numFrames;
        float frameTime;
    };

    class AnimationComponent final : public Component
    {
    public:
        AnimationComponent(GameObject* owner);

        void AddAnimation(const std::string& name, const std::string& spriteSheet, int frameWidth, int frameHeight, int numFrames, float frameTime);
        void PlayAnimation(const std::string& name);
        void StopAnimation();

        void Update(float deltaTime) override;
        void Render() const override;

        const std::string& GetCurrentAnimationName() const;

    private:
        std::unordered_map<std::string, dae::Animation> m_animations;
        std::string m_currentAnimation;
        int m_currentFrame = 0;
        float m_elapsedTime = 0.f;
    };
}