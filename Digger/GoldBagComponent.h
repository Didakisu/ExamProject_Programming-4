#pragma once

#include "Component.h"
#include <memory>
#include "TileType.h"
#include "AnimationComponent.h"
#include "StateMachine.h"
#include "Observer.h"

class TileMap;

namespace dae
{
    class State;
    class StateMachine;
    class Scene;
    class GoldBagComponent;

    enum class PushDirection
    {
        Left,
        Right
    };


    class GoldBagObserver: public Observer
    {
    public:
        explicit GoldBagObserver(GoldBagComponent* goldBagComp);
        void OnNotify(const GameObject& gameObject, Event event) override;

    private:
        GoldBagComponent* m_pGoldBagComponent;
    };


    class GoldBagComponent final : public Component, public StateMachine
    {
    public:
        explicit GoldBagComponent(GameObject* owner);
        ~GoldBagComponent() override = default;

        void Update(float deltaTime) override;

        void TryPush(PushDirection dir);

        bool IsGroundBelow() const;
        void Fall(float deltaTime);
        bool HasLanded() const;

        void SetTileMap(TileMap* tileMap);
        int GetFallDistance() const { return m_FallDistance; }
        void ResetHasHitGround();

        int GetFallStartTileY() const;
        void SetFallStartTileY(int tileY);
        int GetCurrentTileY() const;

        void SetFallAnimationTimer(float timer) { m_FallAnimationTimer = timer; }
        void DecreaseFallAnimationTimer(float deltaTime) { m_FallAnimationTimer -= deltaTime; }
        float GetFallAnimationTimer() const { return m_FallAnimationTimer; }

        void InitializeAnimations();

        bool IsShakingAnimationPlaying() const { return m_IsShakingAnimationPlaying; }
        void SetShakingAnimationPlaying(bool val) { m_IsShakingAnimationPlaying = val; }
        bool IsInShakingState() const;

        bool IsInFallingState() const;

        void SetHasStoppedAnimation(bool val) { m_HasStoppedAnimation = val; }
        bool HasStoppedAnimation() const { return m_HasStoppedAnimation; }

        void SetScene(Scene* scene) { m_pScene = scene; }
        Scene* GetScene() const { return m_pScene; }

    protected:
        std::string CheckNextState() override;

    private:
        float m_FallSpeed = 100.f;
        bool m_HasHitGround = false;

        TileMap* m_pTileMap = nullptr;
        int m_FallDistance = 0;
        int m_FallStartTileY{ -1 };
        float m_FallAnimationTimer{ 0.0f };

        AnimationComponent* m_pAnimationComponent = nullptr;
        bool m_IsShakingAnimationPlaying = false;
        bool m_HasStoppedAnimation = false;
        bool m_IsBroken = false;

        Scene* m_pScene = nullptr;

        //pushing logic variables
        bool m_IsBeingPushed = false;
        glm::vec3 m_PushStartPos{};
        glm::vec3 m_PushTargetPos{};
        float m_PushSpeed = 100.0f; 

    };
}

