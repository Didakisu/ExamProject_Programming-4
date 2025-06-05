#include "ScoreComponent.h"
#include "Data.h"

namespace dae
{
	dae::ScoreComponent::ScoreComponent(GameObject* owner, int initialPoints) :
		Component(owner), m_pOwner(owner), m_Points(initialPoints)
	{

	}

	void dae::ScoreComponent::OnNotify(const GameObject& , Event /*event*/)
	{
		/*if (event == Event::EVENT_PLAYER_COLLECT_ITEM)
		{
			std::cout << "Collected an item!" << std::endl;
			m_Points += 10;
		}*/
	}
}