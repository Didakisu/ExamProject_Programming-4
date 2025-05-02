#include "HealthComponent.h"
#include "Data.h"
#include <ServiceLocator.h>

dae::HealthComponent::HealthComponent(GameObject* owner, int initialLives)
    : Component(owner), m_pOwner(owner), m_Lives(initialLives)
{

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
