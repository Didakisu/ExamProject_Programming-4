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
	//std::cout << "Scene created\n";
	//auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	////create game states manager

	//auto s_TileMap = std::make_shared<TileMap>();

	//dae::LevelLoader loader;
	////loader.LoadLevel("D:/3_Third Year/2nd Semester/Programming 4/2DAE10_Programming4_01_Radeva_Dimana/Data/level.txt", scene, *s_TileMap); //fix this
	//loader.LoadLevel("../Data/level.txt", scene, *s_TileMap);

	////player
	//auto pCharacter = std::make_shared<dae::GameObject>();
	//auto playerComp = pCharacter->AddComponent<dae::PlayerComponent>(scene, s_TileMap);
	//playerComp->Initialize({ s_TileMap->TILE_WIDTH * 7.f, s_TileMap->TILE_HEIGHT * 10.f, 3.f });
	//playerComp->BindInput();
	//scene.Add(pCharacter);
	//
	////enemySpawner setup
	///*const auto& spawnPoints = loader.GetEnemySpawnPositions();
	//auto enemySpawnerGO = std::make_shared<dae::GameObject>();
	//enemySpawnerGO->AddComponent<dae::EnemySpawner>(scene, s_TileMap, spawnPoints, 1, 5.0f);
	//enemySpawnerGO->AddComponent<dae::Transform>();
	//scene.Add(enemySpawnerGO);*/

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