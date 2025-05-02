#include "ServiceLocator.h"
#include "NullSoundSystem.h"
#include <iostream>

namespace dae
{
    std::unique_ptr<SoundSystem> ServiceLocator::m_SoundSystem = std::make_unique<NullSoundSystem>();

    void ServiceLocator::ProvideSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
    {
        m_SoundSystem.reset();
        m_SoundSystem = std::move(soundSystem);
    }

    SoundSystem* ServiceLocator::GetSoundSystem()
    {
        return m_SoundSystem.get();
    }
}