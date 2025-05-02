#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
    class ServiceLocator
    {
    public:
        static void ProvideSoundSystem(std::unique_ptr<SoundSystem> soundSystem); 
        static SoundSystem* GetSoundSystem(); 

    private:
        static std::unique_ptr<SoundSystem> m_SoundSystem; 
    };
}