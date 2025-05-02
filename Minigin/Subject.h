#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Observer.h"
#include <vector>
#include <memory>

namespace dae
{
    class Subject
    {
    public:
        void AddObserver(std::shared_ptr<Observer> observer)
        {
            m_Observers.push_back(observer);
        }

        void RemoveObserver(std::shared_ptr<Observer> observer)
        {
            m_Observers.erase(
                std::remove(m_Observers.begin(), m_Observers.end(), observer),
                m_Observers.end());
        }

        void NotifyObservers(const GameObject& gameObject, Event event)
        {
            for (auto& observer : m_Observers)
            {
                observer->OnNotify(gameObject, event);
            }
        }

    private:
        std::vector<std::shared_ptr<Observer>> m_Observers;
    };
}