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



void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//TileMap tileMap;
	auto s_TileMap = std::make_shared<TileMap>();
	dae::LevelLoader loader;
	loader.LoadLevel("D:/3_Third Year/2nd Semester/Programming 4/2DAE10_Programming4_01_Radeva_Dimana/Data/level.txt", scene, *s_TileMap);

	//character
	//auto pCharacter_2 = std::make_shared<dae::GameObject>();
	//pCharacter_2->AddComponent<dae::DirectionComponent>(); pCharacter_2->AddComponent<dae::RenderComponent>("MainCharacter.png", 32, 32);
	//pCharacter_2->AddComponent<dae::Transform>()->SetLocalPosition(s_TileMap->TILE_WIDTH * 10.f, s_TileMap->TILE_HEIGHT * 2.f, 3.f);

	//auto pAnimComp = pCharacter_2->AddComponent<dae::AnimationComponent>();
	//pAnimComp->AddAnimation("Run", "MainCharacter.png", 32 , 32 , 4, 0.15f);
	//pAnimComp->PlayAnimation("Run");
	////
	//pCharacter_2->AddComponent<dae::CollisionComponent>(32.f, 32.f , &scene);

	//auto pHoleBehindCharacter = std::make_shared<dae::GameObject>();
	//pHoleBehindCharacter->AddComponent<dae::RenderComponent>("tile.png", 35, 28);
	//pHoleBehindCharacter->AddComponent<dae::Transform>();
	//pHoleBehindCharacter->GetTransform()->SetLocalPosition(0.f, 0.f, -2.f);
	//pHoleBehindCharacter->SetParent(pCharacter_2.get() , false);

	//scene.Add(pHoleBehindCharacter);
	//scene.Add(pCharacter_2);
	//
	//dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Pressed, std::make_unique<MoveCommand>(pCharacter_2.get(), 100.0f, glm::vec2{ 0 , -1 } , s_TileMap , scene));
	//dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Pressed, std::make_unique<MoveCommand>(pCharacter_2.get(), 100.0f, glm::vec2{ 0 , 1 }, s_TileMap, scene));
	//dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, dae::InputState::Pressed, std::make_unique<MoveCommand>(pCharacter_2.get(), 100.0f, glm::vec2{ -1 , 0 }, s_TileMap, scene));
	//dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, dae::InputState::Pressed, std::make_unique<MoveCommand>(pCharacter_2.get(), 100.0f, glm::vec2{ 1 , 0 }, s_TileMap, scene));

	//dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_X, dae::InputState::Down, std::make_unique<KillPlayerCommand>(pCharacter_2.get()));
	//
	//auto pScoreComponent_2 = pCharacter_2->AddComponent<dae::ScoreComponent>(0);

	//dae::EventManager::GetInstance().AddObserver(pScoreComponent_2, { EVENT_PLAYER_COLLECT_ITEM });

	//auto sound = dae::ServiceLocator::GetSoundSystem();
	//sound->LoadSound(DEATH_SOUND_ID, "Data_death.wav");

	auto pCharacter = std::make_shared<dae::GameObject>();

	auto playerComp = pCharacter->AddComponent<dae::PlayerComponent>(scene, s_TileMap);
	playerComp->Initialize({ s_TileMap->TILE_WIDTH * 10.f, s_TileMap->TILE_HEIGHT * 2.f, 3.f });
	playerComp->BindInput();

	scene.Add(pCharacter);

	// Setup hole behind player if needed similarly, attach as child etc.





















	//gem
	/*std::vector<dae::GameObject*> collectibles;
	auto pEmeraldCollectible = std::make_shared<dae::GameObject>();
	pEmeraldCollectible->AddComponent<dae::RenderComponent>("Emerald.png", 30, 32);
	pEmeraldCollectible->AddComponent<dae::Transform>()->SetLocalPosition(300.f, 300.f, 0.f);

	std::shared_ptr<dae::Observer>collectible = std::make_shared<dae::CollectibleComponent>(pEmeraldCollectible.get());

	auto collision = pEmeraldCollectible->AddComponent<dae::CollisionComponent>(30.f, 32.f, &scene);

	collision->AddObserver(collectible);
	scene.Add(pEmeraldCollectible);*/

	/*auto pTextInstructions_1 = std::make_shared<dae::GameObject>();
	auto fontInstructions = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	pTextInstructions_1->AddComponent<dae::TextComponent>("Use WASD to move the character on the right, X to inflict damage , C to collect coins!", fontInstructions);
	pTextInstructions_1->AddComponent<dae::Transform>()->SetPosition(10, 35, 0);
	scene.Add(pTextInstructions_1);

	auto pTextInstructions_2 = std::make_shared<dae::GameObject>();
	pTextInstructions_2->AddComponent<dae::TextComponent>("Use the D-pad to move the character on the left, A to inflict damage , B to collect coins!", fontInstructions);
	pTextInstructions_2->AddComponent<dae::Transform>()->SetPosition(10, 15, 0);
	scene.Add(pTextInstructions_2);

	auto pTextInstructionsAudio = std::make_shared<dae::GameObject>();
	pTextInstructionsAudio->AddComponent<dae::TextComponent>("When the player dies(takes damage, and left lives are 0, a sound will be played)", fontInstructions);
	pTextInstructionsAudio->AddComponent<dae::Transform>()->SetPosition(10, 55, 0);
	scene.Add(pTextInstructionsAudio);*/

	//auto pHealthComponent_1 = pCharacter_2->AddComponent<dae::HealthComponent>(3);
	//auto pScoreComponent_1 = pCharacter_2->AddComponent<dae::ScoreComponent>(0);

	//auto pHUD = std::make_shared<dae::GameObject>();

	//auto pHealthTextGO_1 = std::make_shared<dae::GameObject>();
	//auto pHealthText_1 = pHealthTextGO_1->AddComponent<dae::TextComponent>("#lives: 3", fontInstructions);
	//auto pHealthTransform_1 = pHealthTextGO_1->AddComponent<dae::Transform>();
	//pHealthTransform_1->SetPosition(10.f, 250.f, 0.f);

	//auto pScoreTextGO_1 = std::make_shared<dae::GameObject>();
	//auto pScoreText_1 = pScoreTextGO_1->AddComponent<dae::TextComponent>("Score: 0", fontInstructions);
	//auto pScoreTransform_1 = pScoreTextGO_1->AddComponent<dae::Transform>();
	//pScoreTransform_1->SetPosition(10.f, 275.f, 0.f);

	//auto pHUDObserver_1 = std::make_shared<dae::HUDObserver>(pHUD.get(), pHealthText_1, pScoreText_1, pHealthComponent_1, pScoreComponent_1);
	//pHealthComponent_1->AddObserver(pHUDObserver_1);
	////pScoreComponent_1->AddObserver(pHUDObserver_1);

	//scene.Add(pHealthTextGO_1);
	//scene.Add(pScoreTextGO_1);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}
