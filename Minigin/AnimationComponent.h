#pragma once
#include "Component.h"

namespace dae
{
    class AnimationComponent final : public Component
    {
    public:
        AnimationComponent(GameObject* owner, int frameWidth, int frameHeight, int numFrames, float frameTime);

        void Update(float deltaTime) override;
        void Render() const override;

    private:
        int m_frameWidth;
        int m_frameHeight;
        int m_numFrames;
        float m_frameTime;
        float m_elapsedTime;
        int m_currentFrame;
    };
}
