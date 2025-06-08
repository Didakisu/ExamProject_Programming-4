#include "HealthComponent.h"
#include "Data.h"
#include <ServiceLocator.h>
#include "EventManager.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int initialLives)
    : Component(owner), m_pOwner(owner), m_Lives(initialLives)
{
    EventManager::GetInstance().AddObserver(this, { EVENT_PLAYER_GAINED_LIFE });
    //std::cout << "HealthComponent added observer\n";
}

dae::HealthComponent::~HealthComponent()
{
    EventManager::GetInstance().RemoveObserver(this);
    //std::cout << "HealthComponent destroyed and observer removed\n";
}

void dae::HealthComponent::LoseLife()
{
    if (m_Lives > 0)
    {
        m_Lives -= 1;
        NotifyObservers(*m_pOwner, EVENT_PLAYER_LOSING_LIFE);

        if (m_Lives == 0)
        {
            auto soundSystem = dae::ServiceLocator::GetSoundSystem();
            soundSystem->Play(DEATH_SOUND_ID, 50);
        }
    }
}

void dae::HealthComponent::OnNotify(const GameObject& /*gameObject*/, Event event)
{
    if (event == EVENT_PLAYER_GAINED_LIFE)
    {
        m_Lives += 1;
    }
}