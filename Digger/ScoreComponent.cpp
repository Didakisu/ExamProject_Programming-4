#include "ScoreComponent.h"
#include "Data.h"
#include "CollectibleComponent.h"

namespace dae
{
	dae::ScoreComponent::ScoreComponent(GameObject* owner, int initialPoints) :
		Component(owner), m_pOwner(owner), m_Points(initialPoints)
	{

	}

    void dae::ScoreComponent::OnNotify(const GameObject& gameObject, Event event)
    {
       /* if (event == EVENT_PLAYER_COLLECT_ITEM)
        {
    	    m_Points += 10;
    	    std::cout << "points added " << m_Points << std::endl;
        }*/
        if (event == EVENT_PLAYER_COLLECT_ITEM)
        {
            std::cout << "OnNotify received event from sender GameObject address: " << &gameObject << std::endl;

            auto collectible = gameObject.GetComponent<CollectibleComponent>();
            if (collectible)
            {
                std::cout << "CollectibleComponent found on sender GameObject address: " << &gameObject << std::endl;
                int points = collectible->GetPoints();
                m_Points += points;
                std::cout << "Collected points: " << points << ", Total: " << m_Points << std::endl;
            }
            else
            {
                std::cerr << "ERROR: Sender GameObject has no CollectibleComponent! Sender address: " << &gameObject << std::endl;
            }
        }
    }

}