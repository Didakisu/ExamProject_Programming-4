#include "HealthComponent.h"
#include "Data.h"
#include <ServiceLocator.h>
#include "EventManager.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int& sharedLives)
    : Component(owner), m_pOwner(owner), m_Lives(sharedLives)
{
    EventManager::GetInstance().AddObserver(this, { EVENT_PLAYER_GAINED_LIFE });
}

dae::HealthComponent::~HealthComponent()
{
    EventManager::GetInstance().RemoveObserver(this);
}

void dae::HealthComponent::Update(float deltaTime)
{
    if (m_TimerCounting)
    {
        m_RemainingTime -= deltaTime;
        if (m_RemainingTime < 0)
        {
            EventManager::GetInstance().FireEvent(EVENT_PLAYER_OUT_OF_LIVES, GetOwner());
        }
    }
}

void dae::HealthComponent::LoseLife()
{
    if (m_Lives > 0)
    {
        m_Lives -= 1;
        NotifyObservers(*m_pOwner, EVENT_PLAYER_LOSING_LIFE);
        std::cout << "Lives: " << m_Lives << std::endl;
        auto soundSystem = dae::ServiceLocator::GetSoundSystem();
        soundSystem->Play(DEATH_SOUND_ID, 50);

        if (m_Lives == 0)
        {
            std::cout << "Lives are 0!" << std::endl;
            m_RemainingTime = 2.0f;
            m_TimerCounting = true;
            std::cout << "notifying the observers, that the player is out of lives" << std::endl;
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