#include "SDLMixerSoundSystem.h"
#include <SDL_mixer.h>
#include "SDL.h"
#include <iostream>
#include <unordered_map>
#include <thread>
#include <queue>
#include <mutex>

//https://stackoverflow.com/questions/15278343/c11-thread-safe-queue/16075550#16075550 

template <class T>
class SafeQueue
{
public:
    SafeQueue(void)
        : q()
        , m()
        , c()
    {
    }

    ~SafeQueue(void)
    {
    }

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T dequeue(void)
    {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty())
        {
            // release lock as long as the wait and reaquire it afterwards.
            c.wait(lock);
        }
        T val = q.front();
        q.pop();
        return val;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

namespace dae
{
    enum class SoundEvent
    {
        init, 
        load,
        play,
        stop,
        pause,
        resume,
        stopAll,
        quit
    };

    struct SoundInfo
    {
        sound_id soundId;
        int volume;
        std::string path;
    };

    class SDLMixerSoundSystem::SDLMixerSoundSystemImpl
    {
    public:
        SDLMixerSoundSystemImpl(const std::string& dataPath):
            m_DataPath{dataPath}
        {
            m_IsThreadRunning = true;
            m_Thread = std::thread(&SDLMixerSoundSystemImpl::ThreadProcessing , this);

            m_SoundQueue.enqueue({ SoundEvent::init , {} });
        }

        ~SDLMixerSoundSystemImpl()
        {
            m_SoundQueue.enqueue({ SoundEvent::quit , {} });
            m_Thread.join();
        }

        void LoadSound(sound_id id, const std::string& path)
        {
            m_SoundQueue.enqueue({ SoundEvent::load , {id, 0 ,path} });
        }

        void Play(sound_id id, int volume)
        {
            m_SoundQueue.enqueue({ SoundEvent::play , {id , volume} });
        }

        void Stop(sound_id id)
        {
            m_SoundQueue.enqueue({ SoundEvent::stop , {id} });
        }

        void Pause(sound_id id)
        {
            m_SoundQueue.enqueue({ SoundEvent::pause , {id} });
        }

        void Resume(sound_id id)
        {
            m_SoundQueue.enqueue({ SoundEvent::resume , {id} });
        }

        void StopAll()
        {
            m_SoundQueue.enqueue({ SoundEvent::stopAll , {} });
        }

    private:
        std::unordered_map<sound_id, Mix_Chunk*> m_Sounds;
        std::thread m_Thread;
        bool m_IsThreadRunning;
        std::string m_DataPath;
        SafeQueue<std::pair<SoundEvent, SoundInfo>> m_SoundQueue;
        void ThreadProcessing() 
        {
            while(m_IsThreadRunning)
            {
                auto [event , info] = m_SoundQueue.dequeue();
                auto soundId = info.soundId;
                auto volume = info.volume;
                auto path = info.path;

                switch (event)
                {
                case SoundEvent::init:
                {
                    SDL_Init(SDL_INIT_AUDIO);
                    Mix_Init(MIX_INIT_MP3);
                    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                    {
                        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << '\n';
                    }
                    break;
                }
                case SoundEvent::load:
                {
                    if (m_Sounds.contains(soundId))
                    {
                        std::cerr << "Warning: Sound ID already exists. Overwriting: " << soundId << '\n';
                        Mix_FreeChunk(m_Sounds[soundId]);
                    }
                    Mix_Chunk* chunk = Mix_LoadWAV((m_DataPath + "/" + path).c_str());
                    if (!chunk)
                    {
                        std::cerr << "Failed to load sound: " << path << " Error: " << Mix_GetError() << '\n';
                        return;
                    }

                    m_Sounds[soundId] = chunk;
                    break;
                }
                case SoundEvent::play:
                {
                    auto it = m_Sounds.find(soundId);
                    if (it != m_Sounds.end())
                    {
                        Mix_VolumeChunk(it->second, volume);
                        Mix_PlayChannel(-1, it->second, 0);
                    }
                    else
                    {
                        std::cerr << "Sound ID not found: " << soundId << '\n';
                    }
                    break;
                }
                case SoundEvent::stop:
                {
                    for (int i = 0; i < Mix_AllocateChannels(-1); ++i)
                    {
                        if (Mix_GetChunk(i) == m_Sounds[soundId])
                        {
                            Mix_HaltChannel(i);
                        }
                    }
                    break;
                }
                case SoundEvent::pause:
                {
                    for (int i = 0; i < Mix_AllocateChannels(-1); ++i)
                    {
                        if (Mix_GetChunk(i) == m_Sounds[soundId])
                        {
                            Mix_Pause(i);
                        }
                    }
                    break;
                }
                case SoundEvent::resume:
                {
                    for (int i = 0; i < Mix_AllocateChannels(-1); ++i)
                    {
                        if (Mix_GetChunk(i) == m_Sounds[soundId])
                        {
                            Mix_Resume(i);
                        }
                    }
                    break;
                }
                case SoundEvent::stopAll:
                {
                    Mix_HaltChannel(-1);
                    break;
                }
                case SoundEvent::quit:
                {
                    for (auto& pair : m_Sounds)
                    {
                        Mix_FreeChunk(pair.second);
                    }
                    m_Sounds.clear();
                    m_IsThreadRunning = false;

                    Mix_CloseAudio();

                    break;
                }
                }
            }
        }
    };

    SDLMixerSoundSystem::SDLMixerSoundSystem(const std::string& dataPath)
        : m_pImpl(std::make_unique<SDLMixerSoundSystemImpl>(dataPath))
    {
    }

    SDLMixerSoundSystem::~SDLMixerSoundSystem() = default;

    void SDLMixerSoundSystem::LoadSound(sound_id id, const std::string& path)
    {
        m_pImpl->LoadSound(id, path);
    }

    void SDLMixerSoundSystem::Play(sound_id id, int volume)
    {
        m_pImpl->Play(id, volume);
    }

    void SDLMixerSoundSystem::Stop(sound_id id)
    {
        m_pImpl->Stop(id);
    }

    void SDLMixerSoundSystem::Pause(sound_id id)
    {
        m_pImpl->Pause(id);
    }

    void SDLMixerSoundSystem::Resume(sound_id id)
    {
        m_pImpl->Resume(id);
    }

    void SDLMixerSoundSystem::StopAll()
    {
        m_pImpl->StopAll();
    }
}