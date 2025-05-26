#pragma once

#include "State.h" 
#include <memory>

namespace dae
{
    class GoldBagComponent;

    class GoldBagRestingState : public State
    {
    public:
        void OnEnter() override;
        void Update(float deltaTime) override;

    private:
        GoldBagComponent* m_Bag{};
    public:
        explicit GoldBagRestingState(GoldBagComponent* bag) : m_Bag(bag) {}
    };

    class GoldBagShakingState : public State
    {
    public:
        void OnEnter() override;
        void Update(float deltaTime) override;

    private:
        GoldBagComponent* m_Bag{};
    public:
        explicit GoldBagShakingState(GoldBagComponent* bag) : m_Bag(bag) {}
    };

    class GoldBagFallingState : public State
    {
    public:
        void OnEnter() override;
        void Update(float deltaTime) override;

    private:
        GoldBagComponent* m_Bag{};
    public:
        explicit GoldBagFallingState(GoldBagComponent* bag) : m_Bag(bag) {}
    };

    class GoldBagBreakingState : public State
    {
    public:
        explicit GoldBagBreakingState(GoldBagComponent* bag, float breakTime = 0.3f);

        void OnEnter() override;
        void Update(float deltaTime) override;

        bool IsDone() const;

    private:
        GoldBagComponent* m_Bag{};
        float m_BreakTimer{};
        bool m_IsFinished{ false };
    };

    class GoldBagCollectibleState : public State
    {
    public:
        explicit GoldBagCollectibleState(GoldBagComponent* bag);

        void OnEnter() override;

    private:
        GoldBagComponent* m_Bag{};
    };
}
