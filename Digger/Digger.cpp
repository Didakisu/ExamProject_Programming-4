//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>

#endif
#endif

#include <SDL_scancode.h>
#include "Minigin.h"
#include "SceneManager.h"
#include "GameObject.h"  
#include "GameController.h"


void load()
{
	auto& rootScene = dae::SceneManager::GetInstance().CreateScene("RootScene");

	auto gameControllerGO = std::make_shared<dae::GameObject>();
	gameControllerGO->AddComponent<dae::GameController>();
	rootScene.Add(gameControllerGO);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}