#include "CherryPowerUp.h"
#include "PlayerComponent.h"
#include "Data.h"

namespace dae
{
    CherryPowerUp::CherryPowerUp(GameObject* owner)
        : m_Owner(owner)
        , m_IsCollected(false)
    {
      
    }

    void CherryPowerUp::OnNotify(const GameObject& gameObject, Event event)
    {
        if (event != EVENT_COLLISION)
            return;

        if (m_IsCollected)
            return;

        if (gameObject.HasComponent<PlayerComponent>())
        {
            m_IsCollected = true;
            dae::EventManager::GetInstance().FireEvent(EVENT_CHERRY_COLLECTED, m_Owner);
            m_Owner->MarkForDestruction();
        }
    }
}
