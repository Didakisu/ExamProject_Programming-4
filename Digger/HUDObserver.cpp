#include "HUDObserver.h"
#include "HealthComponent.h"
#include "Data.h"
#include <string>

namespace dae
{
    HUDObserver::HUDObserver(GameObject* owner, TextComponent* healthText, TextComponent* scoreText, HealthComponent* health, ScoreComponent* score)
        : Component(owner), m_pHealthTextComponent(healthText), m_pScoreTextComponent(scoreText), m_pHealthComponent(health), m_pScoreComponent(score)
    {

    }

    void HUDObserver::OnNotify(const GameObject& gameObject, Event event)
    {
        (void)gameObject;
        if (event == EVENT_PLAYER_LOSING_LIFE)
        {
            int remainingLives = m_pHealthComponent->GetLives();
            m_pHealthTextComponent->SetText("#lives: " + std::to_string(remainingLives));
        }
        if (event == EVENT_PLAYER_COLLECT_ITEM)
        {
            int currentPoints = m_pScoreComponent->GetPoints();
            m_pScoreTextComponent->SetText("Score: " + std::to_string(currentPoints));
        }
    }
}