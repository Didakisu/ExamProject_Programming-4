#pragma once
#include "SoundSystem.h"

namespace dae
{
    class NullSoundSystem : public SoundSystem
    {
    public:
        void Play(sound_id, int ) override {}
        void Stop(sound_id ) override {}
        void Pause(sound_id ) override {}
        void Resume(sound_id ) override {}
        void StopAll() override {}
        void LoadSound(sound_id , const std::string& ) override {}
    };
}