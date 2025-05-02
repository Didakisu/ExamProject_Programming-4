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
#include "CollectibleManager.h"


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//background
	auto pBackgroundImage = std::make_shared<dae::GameObject>();
	pBackgroundImage->AddComponent<dae::RenderComponent>("background.tga",940, 580);
	pBackgroundImage->AddComponent<dae::Transform>()->SetPosition(0, 0, 0);
	scene.Add(pBackgroundImage);

	//character
	auto pCharacter_2 = std::make_shared<dae::GameObject>();
	pCharacter_2->AddComponent<dae::DirectionComponent>();
	auto pRenderComp = pCharacter_2->AddComponent<dae::RenderComponent>("player_sprites.png", 128, 32);
	pCharacter_2->AddComponent<dae::Transform>()->SetLocalPosition(400.f, 300.f, 0.f);
	pCharacter_2->AddComponent<dae::AnimationComponent>(pRenderComp->GetWidth() / 4,pRenderComp->GetHeight(),4,0.15f);
	//
	pCharacter_2->AddComponent<dae::CollisionComponent>(32.f, 32.f , &scene); // adjust to sprite size

	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Pressed, std::make_unique<MoveCommand>(pCharacter_2.get(), 100.0f, glm::vec2{ 0 , -1 }));
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Pressed, std::make_unique<MoveCommand>(pCharacter_2.get(), 100.0f, glm::vec2{ 0 , 1 }));
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, dae::InputState::Pressed, std::make_unique<MoveCommand>(pCharacter_2.get(), 100.0f, glm::vec2{ -1 , 0 }));
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, dae::InputState::Pressed, std::make_unique<MoveCommand>(pCharacter_2.get(), 100.0f, glm::vec2{ 1 , 0 }));

	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_X, dae::InputState::Down, std::make_unique<KillPlayerCommand>(pCharacter_2.get()));
	

	auto pScoreComponent_2 = pCharacter_2->AddComponent<dae::ScoreComponent>(0);

	dae::EventManager::GetInstance().AddObserver(pScoreComponent_2, { EVENT_PLAYER_COLLECT_ITEM });

	auto sound = dae::ServiceLocator::GetSoundSystem();
	sound->LoadSound(DEATH_SOUND_ID, "Data_death.wav");

	auto pTextInstructions_1 = std::make_shared<dae::GameObject>();
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
	scene.Add(pTextInstructionsAudio);

	auto pHealthComponent_1 = pCharacter_2->AddComponent<dae::HealthComponent>(3);
	auto pScoreComponent_1 = pCharacter_2->AddComponent<dae::ScoreComponent>(0);

	auto pHUD = std::make_shared<dae::GameObject>();

	auto pHealthTextGO_1 = std::make_shared<dae::GameObject>();
	auto pHealthText_1 = pHealthTextGO_1->AddComponent<dae::TextComponent>("#lives: 3", fontInstructions);
	auto pHealthTransform_1 = pHealthTextGO_1->AddComponent<dae::Transform>();
	pHealthTransform_1->SetPosition(10.f, 250.f, 0.f);

	auto pScoreTextGO_1 = std::make_shared<dae::GameObject>();
	auto pScoreText_1 = pScoreTextGO_1->AddComponent<dae::TextComponent>("Score: 0", fontInstructions);
	auto pScoreTransform_1 = pScoreTextGO_1->AddComponent<dae::Transform>();
	pScoreTransform_1->SetPosition(10.f, 275.f, 0.f);

	auto pHUDObserver_1 = std::make_shared<dae::HUDObserver>(pHUD.get(), pHealthText_1, pScoreText_1, pHealthComponent_1, pScoreComponent_1);
	pHealthComponent_1->AddObserver(pHUDObserver_1);
	//pScoreComponent_1->AddObserver(pHUDObserver_1);

	scene.Add(pCharacter_2);
	scene.Add(pHealthTextGO_1);
	scene.Add(pScoreTextGO_1);
	
	//emerald collectible
	std::vector<dae::GameObject*> collectibles;
	auto pEmeraldCollectible = std::make_shared<dae::GameObject>();
	pEmeraldCollectible->AddComponent<dae::RenderComponent>("Emerald.png", 30, 32);
	pEmeraldCollectible->AddComponent<dae::Transform>()->SetLocalPosition(300.f, 300.f, 0.f);

	std::shared_ptr<dae::Observer>collectible = std::make_shared<dae::CollectibleComponent>(pEmeraldCollectible.get());//pEmeraldCollectible->AddComponent<dae::CollectibleComponent>();
	//collectibles.push_back(pEmeraldCollectible.get());
	auto collision = pEmeraldCollectible->AddComponent<dae::CollisionComponent>(30.f, 32.f , &scene);
	
	collision->AddObserver(collectible);

	//dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_C, dae::InputState::Down, std::make_unique<CollectCommand>(pCharacter_2.get() , collectibles));

	scene.Add(pEmeraldCollectible);

	
	//logo

	/*auto pLogoImage = std::make_shared<dae::GameObject>();
	pLogoImage->AddComponent<dae::RenderComponent>("logo.tga");
	pLogoImage->AddComponent<dae::Transform>()->SetPosition(300, 100, 0);
	scene.Add(pLogoImage);*/

	//text

	/*auto pTextImage = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pTextImage->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);

	pTextImage->AddComponent<dae::Transform>()->SetPosition(100, 50, 0);
	scene.Add(pTextImage);*/

	//fps

	/*auto pFPSObject = std::make_shared<dae::GameObject>();
	auto pFPSFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pFPSObject->AddComponent<dae::TextComponent>("FPS: 0", pFPSFont);
	pFPSObject->AddComponent<dae::Transform>()->SetPosition(10, 10, 0);

	pFPSObject->AddComponent<dae::FPSComponent>();
	scene.Add(pFPSObject);*/

	//add instructions 

	

	/*auto pHealthComponent_2 = pCharacter_2->AddComponent<dae::HealthComponent>(3);
	auto pScoreComponent_2 = pCharacter_2->AddComponent<dae::ScoreComponent>(0);

	auto pHealthTextGO_2 = std::make_shared<dae::GameObject>();
	auto pHealthText_2 = pHealthTextGO_2->AddComponent<dae::TextComponent>("#lives: 3", fontInstructions);
	auto pHealthTransform_2 = pHealthTextGO_2->AddComponent<dae::Transform>();
	pHealthTransform_2->SetPosition(10.f, 300.f, 0.f);

	auto pScoreTextGO_2 = std::make_shared<dae::GameObject>();
	auto pScoreText_2 = pScoreTextGO_2->AddComponent<dae::TextComponent>("Score: 0", fontInstructions);
	auto pScoreTransform_2 = pScoreTextGO_2->AddComponent<dae::Transform>();
	pScoreTransform_2->SetPosition(10.f, 325.f, 0.f);

	auto pHUDObserver_2 = std::make_shared<dae::HUDObserver>(pHUD.get(), pHealthText_2, pScoreText_2, pHealthComponent_2, pScoreComponent_2);
	pHealthComponent_2->AddObserver(pHUDObserver_2);
	pScoreComponent_2->AddObserver(pHUDObserver_2);

	scene.Add(pHealthTextGO_2);
	scene.Add(pScoreTextGO_2);*/

	//rotation

	/*float orbitSpeed = 0.05f;
	float orbitRadius = 50.f;

	auto pCenterObject = std::make_shared<dae::GameObject>();
	pCenterObject->AddComponent<dae::Transform>()->SetLocalPosition(200.f,300.f,0.f);
	scene.Add(pCenterObject);

	auto pRotatingCharacter_1 = std::make_shared<dae::GameObject>();
	pRotatingCharacter_1->AddComponent<dae::RenderComponent>("test_1.png");
	pRotatingCharacter_1->AddComponent<dae::Transform>()->SetLocalPosition(0.f, 0.f, 0.f);

	pRotatingCharacter_1->SetParent(pCenterObject.get(), true);

	pRotatingCharacter_1->AddComponent<dae::RotatorComponent>(orbitRadius, orbitSpeed);

	scene.Add(pRotatingCharacter_1);

	orbitSpeed = -0.09f;
	orbitRadius = 25.f;

	auto pRotatingCharacter_2 = std::make_shared<dae::GameObject>();
	pRotatingCharacter_2->AddComponent<dae::RenderComponent>("test_2.png");
	pRotatingCharacter_2->AddComponent<dae::Transform>()->SetLocalPosition(0.f, 0.f, 0.f);

	pRotatingCharacter_2->SetParent(pRotatingCharacter_1.get(), true);

	pRotatingCharacter_2->AddComponent<dae::RotatorComponent>(orbitRadius, orbitSpeed);


	scene.Add(pRotatingCharacter_2);*/
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}
