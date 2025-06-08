#include "EventManager.h"
#include <iostream>

void dae::EventManager::FireEvent(Event id, GameObject* sender, GameObject* receiver)
{
	eventQueue.push({id, sender, receiver});
}

void dae::EventManager::ProcessEvents()
{
    while (!eventQueue.empty())
    {
        EventWrapper e = eventQueue.front();
        eventQueue.pop();

        if (e.receiver == nullptr)
        {
            for (auto& observer : m_Observers)
            {
                auto result = std::find(observer.events.begin(), observer.events.end(), e.id);

                if (result != observer.events.end())
                {
                    observer.observerPtr->OnNotify(*e.sender, e.id);
                }
            }
        }
        else
        {
            for (auto& observer : m_Observers)
            {
                auto pointer = dynamic_cast<Component*>(observer.observerPtr);
                if (pointer)
                {
                    if (pointer->GetOwner() == e.receiver)
                    {
                        observer.observerPtr->OnNotify(*e.sender, e.id);
                    }
                }
            }
        }
    }
}


void dae::EventManager::AddObserver(Observer* observer , std::vector<Event> events)
{
    std::cout << "[EventManager] AddObserver: " << observer << "\n";
    m_Observers.push_back({ events,observer });
}

//void dae::EventManager::RemoveObserver(std::shared_ptr<Observer> observer)
//{
//    m_Observers.erase(
//        std::find_if(m_Observers.begin(), m_Observers.end(), [observer](const ObserverWrapper& o) {return observer.get() == o.observerPtr; }));
//}

void dae::EventManager::RemoveObserver(Observer* observer)
{
    std::cout << "[EventManager] RemoveObserver: " << observer << "\n";

    m_Observers.erase(
        std::remove_if(
            m_Observers.begin(),
            m_Observers.end(),
            [observer](const ObserverWrapper& o) { return o.observerPtr == observer; }
        ),
        m_Observers.end()
    );
}