﻿#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include <chrono>
#include <thread>
#include "SDLMixerSoundSystem.h"
#include "ServiceLocator.h"
#include "LoggingSoundSystem.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		480, //640
		352, //must be 352, becasue of ui
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);
	ServiceLocator::ProvideSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLMixerSoundSystem>(dataPath)));

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& eventManager = EventManager::GetInstance();

	using clock = std::chrono::high_resolution_clock;
	const float fixedTimeStep = 0.016f;  
	float accumulator = 0.0f;

	const float targetFrameTime = 1.0f / 60.0f;  

	auto previousTime = clock::now();
	bool doContinue = true;

	while (doContinue)
	{
		auto currentTime = clock::now();
		std::chrono::duration<float> frameTime = currentTime - previousTime;
		previousTime = currentTime;

		float deltaTime = frameTime.count();
		accumulator += deltaTime;

		sceneManager.Update(deltaTime);

		while (accumulator >= fixedTimeStep)
		{
			sceneManager.FixedUpdate(fixedTimeStep);
			accumulator -= fixedTimeStep;
		}

		doContinue = input.ProcessInput(deltaTime);
		eventManager.ProcessEvents();

		renderer.Render();

		auto frameEndTime = clock::now();
		std::chrono::duration<float> elapsedTime = frameEndTime - currentTime;
		float sleepTime = targetFrameTime - elapsedTime.count();

		if (sleepTime > 0)
		{
			std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
		}
	}

	SceneManager::GetInstance().DeleteScenes();
}
