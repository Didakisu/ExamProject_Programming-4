#include "ScoreComponent.h"
#include "Data.h"
#include "CollectibleComponent.h"
#include "EventManager.h"

namespace dae
{
    dae::ScoreComponent::ScoreComponent(GameObject* owner, int& sharedScore) :
        Component(owner), m_pOwner(owner), m_Points(sharedScore)
    {

    }

    void ScoreComponent::OnNotify(const GameObject& , Event event)
    {
        if (event == EVENT_COLLECTED_GEM)
        {
            EventManager::GetInstance().FireEvent(EVENT_GAME_GEM_COLLECTED, nullptr, nullptr);

            ++m_ConsecutiveGemsCollected;

            if (m_ConsecutiveGemsCollected == 8)
            {
                m_Points += 250;
                m_ConsecutiveGemsCollected = 0;
            }
            else
            {
                m_Points += 25;
            }
        }
        else if (event == EVENT_COLLECTED_GOLD)
        {
            m_ConsecutiveGemsCollected = 0;

            m_Points += 500;
        }

        if (event == EVENT_ENEMY_KILLED_BY_FIREBALL)
        {
            m_Points += 250;
            EventManager::GetInstance().FireEvent(EVENT_GAME_SCORE_CHANGED, GetOwner());
        }

        if (event == EVENT_ENEMY_KILLED_BY_PLAYER_IN_BONUS_STATE)
        {
            m_Points += 400;
            EventManager::GetInstance().FireEvent(EVENT_GAME_SCORE_CHANGED, GetOwner());
        }

        EventManager::GetInstance().FireEvent(EVENT_GAME_SCORE_CHANGED , GetOwner());

        if (m_Points / 20000 > m_LiveCounter)
        {
            EventManager::GetInstance().FireEvent(EVENT_PLAYER_GAINED_LIFE, nullptr, GetOwner());
            m_LiveCounter = m_Points / 20000;
        }
    }
}