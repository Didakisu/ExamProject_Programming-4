#pragma once
#include "SoundSystem.h"
#include <memory>
#include <string>

namespace dae
{
    class SDLMixerSoundSystem final : public SoundSystem
    {
    public:
        SDLMixerSoundSystem(const std::string& dataPath);
        ~SDLMixerSoundSystem() override;

        void LoadSound(sound_id id, const std::string& path) override;
        void Play(sound_id id, int volume) override;
        void Stop(sound_id id) override;
        void Pause(sound_id id) override;
        void Resume(sound_id id) override;
        void StopAll() override;
        void ToggleMute() override;

    private:
        class SDLMixerSoundSystemImpl; 
        std::unique_ptr<SDLMixerSoundSystemImpl> m_pImpl;
    };
}
