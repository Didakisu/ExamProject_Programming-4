#pragma once

namespace dae {
    class GoldBagComponent;

    class GoldBagState {
    public:
        virtual ~GoldBagState() = default;

        virtual void OnEnter(GoldBagComponent& /*bag*/) {}
        virtual void OnExit(GoldBagComponent& /*bag*/) {}
        virtual void Update(GoldBagComponent& /*bag*/, float /*deltaTime*/) = 0;
        virtual bool CanBreak() const { return false; }
    };
}

