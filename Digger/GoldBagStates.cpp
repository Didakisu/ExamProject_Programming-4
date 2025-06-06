#include "GoldBagStates.h"
#include "GoldBagComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "CollectibleComponent.h"
#include "CollisionComponent.h"
#include "Scene.h"
#include <iostream>
#include <memory>

namespace dae
{
    void GoldBagRestingState::OnEnter() 
    {
        auto* collider = m_Bag->GetOwner()->GetComponent<CollisionComponent>();
        if (collider)
        {
            collider->SetIsTrigger(false);
        }
    }

    void GoldBagRestingState::Update(float /*deltaTime*/)
    {
        if (!m_Bag->IsGroundBelow())
        {
            m_Bag->SetFallAnimationTimer(1.0f);
            m_Bag->ResetHasHitGround();
        }
    }


    void GoldBagShakingState::OnEnter()
    {
        auto* anim = m_Bag->GetOwner()->GetComponent<AnimationComponent>();
        auto* collider = m_Bag->GetOwner()->GetComponent<CollisionComponent>();

        if (anim && anim->GetCurrentAnimationName() != "Shaking")
        {
            anim->PlayAnimation("Shaking");
        }
       
        if (collider)
        {
            collider->SetIsTrigger(false);
        }
    }

    void GoldBagShakingState::Update(float deltaTime)
    {
        if (m_Bag->GetFallAnimationTimer() > 0.f)
        {
            m_Bag->DecreaseFallAnimationTimer(deltaTime);
        }
    }


    void GoldBagFallingState::OnEnter()
    {
        auto* collider = m_Bag->GetOwner()->GetComponent<CollisionComponent>();
        auto* anim = m_Bag->GetOwner()->GetComponent<AnimationComponent>();
        if (anim)
        {
            anim->PlayAnimation("Falling");
        }
        if (collider)
        {
            collider->SetIsTrigger(false);
        }
    }

    void GoldBagFallingState::Update(float deltaTime)
    {
        m_Bag->Fall(deltaTime);
    }


    GoldBagBreakingState::GoldBagBreakingState(GoldBagComponent* bag, float breakTime)
        : m_Bag(bag), m_BreakTimer(breakTime)
    {
    }

    void GoldBagBreakingState::OnEnter()
    {
        auto* anim = m_Bag->GetOwner()->GetComponent<AnimationComponent>();
        auto* collider = m_Bag->GetOwner()->GetComponent<CollisionComponent>();
        if (anim)
        {
            anim->PlayAnimation("Breaking");
            std::cout << "BREAK" << std::endl;
        }
        if (collider)
        {
            collider->SetIsTrigger(true);
        }
    }

    void GoldBagBreakingState::Update(float deltaTime)
    {
        m_BreakTimer -= deltaTime;
        if (m_BreakTimer <= 0.f && !m_IsFinished)
        {
            m_IsFinished = true;
        }
    }

    bool GoldBagBreakingState::IsDone() const
    {
        return m_IsFinished;
    }

    GoldBagCollectibleState::GoldBagCollectibleState(GoldBagComponent* bag)
        : m_Bag(bag)
    {
    }

    void GoldBagCollectibleState::OnEnter()
    {
        auto* scene = m_Bag->GetScene();
        if (!scene) throw std::runtime_error("No scene found");

        auto* render = m_Bag->GetOwner()->GetComponent<RenderComponent>();
        if (render)
        {
            if (auto* anim = m_Bag->GetOwner()->GetComponent<AnimationComponent>())
            {
                anim->StopAnimation();
            }
            render->SetTexture("Gold.png");
            render->SetSourceRect({ 30 , 0 , 15 , 18 });
        }

        auto goldCollectible = std::make_shared<CollectibleComponent>(m_Bag->GetOwner() , 20);
        m_Bag->GetOwner()->AddComponent<CollectibleComponent>();

        auto collider = m_Bag->GetOwner()->AddComponent<CollisionComponent>(32.f, 32.f, scene);
        collider->AddObserver(goldCollectible);

    }
}