#include "EnemyStates.h"
#include "EnemyComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "DirectionComponent.h"
#include "TileMap.h"
#include "Physics.h"
#include <iostream>

namespace dae
{
    void EnemyNormalState::OnEnter()
    {
        auto* anim = m_Enemy->GetOwner()->GetComponent<AnimationComponent>();
        if (anim)
        {

            anim->AddAnimation("Walk", "enemy_sprites.png", 64 / 4, 18, 4, 0.15f);
            anim->PlayAnimation("Walk");
        }
    }

    void EnemyNormalState::Update(float deltaTime)
    {
        //std::cout << "State: Normal" << std::endl;
        m_Enemy->HandleWalking(deltaTime);
    }


    void EnemyEnragedState::OnEnter()
    {
        auto* anim = m_Enemy->GetOwner()->GetComponent<AnimationComponent>();
        if (anim)
        {

            anim->AddAnimation("EnragedDig", "EnragedEnemies.png", 64 / 4, 15, 4, 0.15f);
            anim->PlayAnimation("EnragedDig");
        }
    }

    void EnemyEnragedState::Update(float deltaTime)
    {
        //std::cout << "State: Enraged" << std::endl;
        m_Enemy->HandleEnragedBehavior(deltaTime);
    }

    void EnemyDeadState::OnEnter()
    {
        m_Enemy->Die();
        //m_Enemy->GetOwner()->MarkForDestruction();

        auto* anim = m_Enemy->GetOwner()->GetComponent<AnimationComponent>();
        if (anim)
        {

            anim->AddAnimation("Death", "NobbinDeath.png", 16, 16, 1, 0.15f);
            anim->PlayAnimation("Death");
        }
    }

    void EnemyDeadState::Update(float /*deltaTime*/)
    {
        std::cout << "[DEBUG] EnemyDeadState::Update()\n";
        m_Enemy->HandleDeadBehavior(/*deltaTime*/);
    }
}