#pragma once
#include "GoldBagState.h"
#include <memory>
#include <iostream>

namespace dae
{
    class GoldBagRestingState : public GoldBagState
    {
    public:
        //void OnEnter(GoldBagComponent& /*bag*/) override;
        void Update(GoldBagComponent& /*bag*/, float deltaTime) override;
    };

    class GoldBagFallingState : public GoldBagState
    {
    public:
        //void OnEnter(GoldBagComponent& /*bag*/) override;

        void Update(GoldBagComponent& /*bag*/, float deltaTime) override;
    };

    class GoldBagBreakingState : public GoldBagState
    {
    public:
        explicit GoldBagBreakingState(float breakTime = 2.0f)
            : m_BreakTimer(breakTime) {
        }

        //void OnEnter(GoldBagComponent& /*bag*/) override;
        void Update(GoldBagComponent& /*bag*/, float deltaTime) override;
        bool CanBreak() const override { return true; }

    private:
        float m_BreakTimer{};
    };
}
