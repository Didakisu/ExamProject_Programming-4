#include "LoggingSoundSystem.h"

namespace dae
{
    LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem> realSoundSystem)
        : m_RealSoundSystem(std::move(realSoundSystem))
    {
    }

    void LoggingSoundSystem::LoadSound(sound_id id, const std::string& path)
    {
        std::cout << "Loading sound: ID = " << id << ", Path = " << path << '\n';
        m_RealSoundSystem->LoadSound(id, path);
    }

    void LoggingSoundSystem::Play(sound_id id, int volume)
    {
        std::cout << "Playing sound: ID = " << id << ", Volume = " << volume << '\n';
        m_RealSoundSystem->Play(id, volume);
    }

    void LoggingSoundSystem::Pause(sound_id id)
    {
        std::cout << "Pausing sound: ID = " << id << '\n';
        m_RealSoundSystem->Pause(id);
    }

    void LoggingSoundSystem::Resume(sound_id id)
    {
        std::cout << "Resuming sound: ID = " << id << '\n';
        m_RealSoundSystem->Resume(id);
    }

    void LoggingSoundSystem::Stop(sound_id id)
    {
        std::cout << "Stopping sound: ID = " << id << '\n';
        m_RealSoundSystem->Stop(id);
    }

    void LoggingSoundSystem::StopAll()
    {
        std::cout << "Stopping all sounds\n";
        m_RealSoundSystem->StopAll();
    }

    void LoggingSoundSystem::ToggleMute()
    {
        std::cout << "Mute all sounds\n";
        m_RealSoundSystem->ToggleMute();
    }
}