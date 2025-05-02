#pragma once
#include "SoundSystem.h"
#include <iostream>
#include <memory>

namespace dae
{
    class LoggingSoundSystem final : public SoundSystem
    {
    public:
        explicit LoggingSoundSystem(std::unique_ptr<SoundSystem> realSoundSystem);
        ~LoggingSoundSystem() override = default;

        void LoadSound(sound_id id, const std::string& path) override;
        void Play(sound_id id, int volume) override;
        void Pause(sound_id id) override;
        void Resume(sound_id id) override;
        void Stop(sound_id id) override;
        void StopAll() override;

    private:
        std::unique_ptr<SoundSystem> m_RealSoundSystem;
    };
}

