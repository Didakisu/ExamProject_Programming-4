#pragma once
#include <string>

namespace dae
{
    using sound_id = unsigned short; //unique identifier type

    class SoundSystem
    {
    public:
        virtual ~SoundSystem() = default;

        virtual void LoadSound(sound_id id, const std::string& path) = 0;
        virtual void Play(sound_id id, int volume) = 0;
        virtual void Pause(sound_id id) = 0;
        virtual void Resume(sound_id id) = 0;
        virtual void Stop(sound_id id) = 0;
        virtual void StopAll() = 0;
    };
}
