#pragma once
#include "Observer.h"
#include "Component.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"

namespace dae
{
    class HUDObserver : public Component, public Observer
    {
    public:
        HUDObserver(GameObject* owner, TextComponent* healthText, TextComponent* scoreText, HealthComponent* health, ScoreComponent* score);

        void OnNotify(const GameObject& gameObject, Event event) override;

    private:
        TextComponent* m_pHealthTextComponent;
        TextComponent* m_pScoreTextComponent;
        HealthComponent* m_pHealthComponent;
        ScoreComponent* m_pScoreComponent;
    };
}