#include "ScoreComponent.h"
#include "Data.h"

namespace dae
{
	dae::ScoreComponent::ScoreComponent(GameObject* owner, int initialPoints) :
		Component(owner), m_pOwner(owner), m_Points(initialPoints)
	{

	}

	void dae::ScoreComponent::OnNotify(const GameObject& , Event )
	{
		//std::cout << "collected coin!" << std::endl;
		m_Points += 10;
		//NotifyObservers(*m_pOwner, EVENT_PLAYER_LOSING_LIFE);
	}
}