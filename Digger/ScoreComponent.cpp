#include "ScoreComponent.h"
#include "Data.h"
#include "CollectibleComponent.h"
#include "EventManager.h"

namespace dae
{
    dae::ScoreComponent::ScoreComponent(GameObject* owner, int initialPoints) :
        Component(owner), m_pOwner(owner), m_Points(initialPoints)
    {

    }

    void ScoreComponent::OnNotify(const GameObject& /*gameObject*/, Event event)
    {
        if (event == EVENT_COLLECTED_GEM)
        {
            ++m_ConsecutiveGemsCollected;

            if (m_ConsecutiveGemsCollected == 8)
            {
                m_Points += 250;
                std::cout << "Collected 8 gems in a row! total points: " << m_Points << std::endl;
                m_ConsecutiveGemsCollected = 0;
            }
            else
            {
                m_Points += 25;
                std::cout << "Gem collected. Points: " << m_Points << " (" << m_ConsecutiveGemsCollected << " in a row)" << std::endl;
            }
        }
        else if (event == EVENT_COLLECTED_GOLD)
        {
            m_ConsecutiveGemsCollected = 0;

            m_Points += 500;
            std::cout << "Gold collected - points: " << m_Points << std::endl;
        }
        
        if (m_Points / 20000 > m_LiveCounter)
        {
            EventManager::GetInstance().FireEvent(EVENT_PLAYER_GAINED_LIFE, nullptr, GetOwner());
            m_LiveCounter = m_Points / 20000;
        }
    }
}