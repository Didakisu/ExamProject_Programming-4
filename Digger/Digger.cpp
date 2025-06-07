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
#include "ResourceManager.h"
#include "TextObject.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "Scene.h"
#include "GameObject.h"  
#include "RotatorComponent.h"
#include <iostream>
#include "InputManager.h"
#include "HealthComponent.h"
#include "HUDObserver.h"
#include "ScoreComponent.h"
#include "GameCommands.h"
#include "EventManager.h"
#include "Data.h"
#include "ServiceLocator.h"
#include "AnimationComponent.h"
#include "CollectibleComponent.h"
#include <DirectionComponent.h>
#include <CollisionComponent.h>
#include "TileType.h"
#include "TileMap.h"
#include "LevelLoader.h"
#include "GoldBagComponent.h"
#include "GoldBagStates.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "EnemySpawner.h"
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