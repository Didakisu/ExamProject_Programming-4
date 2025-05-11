#include "GoldBagStates.h"
#include "GoldBagComponent.h"

namespace dae
{
    void GoldBagRestingState::Update(GoldBagComponent& bag, float /*deltaTime*/)
    {
        if (!bag.IsGroundBelow())
        {
            bag.SetFallAnimationTimer(2.0f);
            bag.ResetHasHitGround();
            bag.SetState(std::make_unique<GoldBagFallingState>());
        }
    }

    void GoldBagFallingState::Update(GoldBagComponent& bag, float deltaTime)
    {
        if (bag.GetFallAnimationTimer() > 0.f)
        {
            std::cout << "Playing falling animation..." << std::endl;
            bag.DecreaseFallAnimationTimer(deltaTime);
            return; 
        }

        bag.Fall(deltaTime);

        if (bag.HasLanded())
        {
            int distance = bag.GetCurrentTileY() - bag.GetFallStartTileY();
            std::cout << "DISTANCE: " << distance << std::endl;
            std::cout << "CURRENT Y TILE: " << bag.GetCurrentTileY() << std::endl;
            std::cout << "START Y TILE: " << bag.GetFallStartTileY() << std::endl;

            bag.SetFallStartTileY(bag.GetCurrentTileY());

            if (distance >= 3)
            {
                bag.SetState(std::make_unique<GoldBagBreakingState>());
                std::cout << "coin bag broke!" << std::endl;
            }
            else
            {
                bag.SetState(std::make_unique<GoldBagRestingState>());
            }
        }
    }

    void GoldBagBreakingState::Update(GoldBagComponent& bag, float deltaTime)
    {
        m_BreakTimer -= deltaTime;
        if (m_BreakTimer <= 0.f)
        {
            bag.MakeCollectible();
        }
    }
}
