#pragma once
#include "Observer.h"
#include "Component.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"

namespace dae
{

    
    class HUDObserver : public Component, public Observer
    {
    public:
        HUDObserver(dae::GameObject* pOwner, TextComponent* pScoreText, ScoreComponent* pScoreComponent, const std::vector<RenderComponent*>& lifeIcons);
        void OnNotify(const GameObject& sender, Event event) override;
        void SetLifeIcons(const std::vector<RenderComponent*>& lifeIcons) { m_LifeIcons = lifeIcons; }
    private:
        TextComponent* m_pScoreText{};
        ScoreComponent* m_pScoreComponent{};
        HealthComponent* m_pHealthComponent{};
        std::vector<RenderComponent*> m_LifeIcons;
    };
}
