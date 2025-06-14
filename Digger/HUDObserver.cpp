#include "HUDObserver.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Data.h"
#include <string>
#include <iomanip> 
#include <sstream> 

namespace dae
{
    HUDObserver::HUDObserver(GameObject* pOwner, TextComponent* pScoreText, ScoreComponent* pScoreComponent, const std::vector<RenderComponent*>& lifeIcons)
        : Component(pOwner)
        , m_pScoreText(pScoreText)
        , m_pScoreComponent(pScoreComponent)
        , m_LifeIcons(lifeIcons)
    {
        m_pHealthComponent = pOwner->GetComponent<HealthComponent>();


        int lives = m_pHealthComponent->GetLives();

        for (size_t i = 0; i < m_LifeIcons.size(); ++i)
        {
            if (m_LifeIcons[i])
            {
                m_LifeIcons[i]->SetEnabled(static_cast<int>(i) < lives);
            }
        }

        if (m_pScoreComponent && m_pScoreText)
        {
            int score = m_pScoreComponent->GetPoints();
            std::ostringstream oss;
            oss << std::setw(6) << std::setfill('0') << score;
            m_pScoreText->SetText(oss.str());
        }
    }

    void dae::HUDObserver::OnNotify(const GameObject& /*sender*/, Event event)
    {
        if (!m_pScoreComponent || !m_pScoreText)
            return;

        if (event == EVENT_PLAYER_LOSING_LIFE || event == EVENT_PLAYER_GAINED_LIFE)
        {
            int lives = m_pHealthComponent->GetLives(); 

            for (size_t i = 0; i < m_LifeIcons.size(); ++i)
            {
                if (m_LifeIcons[i])
                {
                    m_LifeIcons[i]->SetEnabled(static_cast<int>(i) < lives);
                }
            }
        }

        if (event== EVENT_GAME_SCORE_CHANGED)
        {
            int score = m_pScoreComponent->GetPoints();
            std::ostringstream oss;
            oss << std::setw(6) << std::setfill('0') << score;

            m_pScoreText->SetText(oss.str());
        }
    }
}