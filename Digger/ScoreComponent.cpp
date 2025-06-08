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
            ++m_ConsecutiveGemsCollected;
            ++m_TotalGemsCollected;

            if (m_TotalGemsCollected >= m_TotalGemsInLevel && m_TotalGemsInLevel > 0)
            {
                std::cout << "Collected all gems!" << std::endl;
                EventManager::GetInstance().FireEvent(EVENT_COLLECTED_ALL_GEMS, nullptr, nullptr);
            }

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
        
        if (m_Points / 20000 > m_LiveCounter)
        {
            EventManager::GetInstance().FireEvent(EVENT_PLAYER_GAINED_LIFE, nullptr, GetOwner());
            m_LiveCounter = m_Points / 20000;
        }
    }
}