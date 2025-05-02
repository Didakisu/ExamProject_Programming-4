#pragma once
#include "Singleton.h"
#include <queue>
#include "Observer.h"

namespace dae
{
	struct EventWrapper
	{
		Event id;
		GameObject* sender;
		GameObject* receiver;
	};

	struct ObserverWrapper
	{
		std::vector<Event> events;
		Observer* observerPtr;
	};

	class EventManager: public Singleton<EventManager>
	{
	public:
		void FireEvent(Event id , GameObject* sender, GameObject* receiver = nullptr);
		void AddObserver(Observer* observer, std::vector<Event> events);
		void RemoveObserver(std::shared_ptr<Observer> observer);

	private:
		void ProcessEvents();
		friend class Minigin;

		std::queue<EventWrapper>eventQueue;
        std::vector<ObserverWrapper> m_Observers;
	};
}