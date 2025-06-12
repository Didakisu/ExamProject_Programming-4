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
#include "PlayerComponent.h"

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
        //m_Enemy->HandleWalking(deltaTime);
        m_Enemy->HandleEnragedBehavior(deltaTime);
    }

    void EnemyNormalState::OnExit()
    {

        auto* anim = m_Enemy->GetOwner()->GetComponent<AnimationComponent>();
        if (anim)
        {
            anim->StopAnimation(); 
        }

        m_Enemy->ResetDeadState();
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
        m_Enemy->HandleEnragedBehavior(deltaTime);
    }

    void EnemyDeadState::OnEnter()
    {
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


    void EnemyBonusState::OnEnter()
    {
        std::cout << "[DEBUG] EnemyBonusState: Entered bonus mode\n";

        auto* anim = m_Enemy->GetOwner()->GetComponent<AnimationComponent>();
        if (anim)
        {
            anim->AddAnimation("Walk", "enemy_sprites.png", 64 / 4, 18, 4, 0.15f);
            anim->PlayAnimation("Walk");
        }

        LevelLoader::SetDirtTileTextures("BonusStateBg.png");
    }

    void EnemyBonusState::Update(float deltaTime)
    {
        m_BonusTimer -= deltaTime;
        std::cout << "[EnemyBonusState] Bonus timer = " << m_BonusTimer << " seconds left\n";
        if (m_BonusTimer <= 0.f)
        {
            std::cout << "[EnemyBonusState] Bonus timer expired, ending bonus state\n";
            LevelLoader::SetDirtTileTextures(LevelLoader::GetOriginalDirtTexture());
            m_Enemy->EndBonusState(); 
            
            return;
        }

        //m_Enemy->HandleWalking(deltaTime);
        m_Enemy->HandleEnragedBehavior(deltaTime);
    }

}