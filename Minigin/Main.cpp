#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//background
	auto pBackgroundImage = std::make_shared<dae::GameObject>();
	pBackgroundImage->AddComponent<dae::RenderComponent>("background.tga");
	pBackgroundImage->AddComponent<dae::Transform>()->SetPosition(0, 0 , 0);
	scene.Add(pBackgroundImage);

	//logo
	auto pLogoImage = std::make_shared<dae::GameObject>();
	pLogoImage->AddComponent<dae::RenderComponent>("logo.tga");
	pLogoImage->AddComponent<dae::Transform>()->SetPosition(300, 200, 0);
	scene.Add(pLogoImage);
	//text
	auto pTextImage = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pTextImage->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);

	pTextImage->AddComponent<dae::Transform>()->SetPosition(100, 50, 0);
	scene.Add(pTextImage);

	//fps
	auto pFPSObject = std::make_shared<dae::GameObject>();
	auto pFPSFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pFPSObject->AddComponent<dae::TextComponent>("FPS: 0", pFPSFont);
	pFPSObject->AddComponent<dae::Transform>()->SetPosition(10, 10, 0);

	pFPSObject->AddComponent<dae::FPSComponent>();
	scene.Add(pFPSObject); 

	//moving characters
	auto pCharacter_1 = std::make_shared<dae::GameObject>();
	pCharacter_1->AddComponent<dae::RenderComponent>("test_1.png");
	pCharacter_1->AddComponent<dae::Transform>()->SetLocalPosition(200.f, 300.f, 0.f);

	dae::InputManager::GetInstance().BindGamepadCommand(dae::Gamepad::GamePadButton::DPadUp, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(pCharacter_1.get(), 200.0f, glm::vec2{ 0 , -1 }));
	dae::InputManager::GetInstance().BindGamepadCommand(dae::Gamepad::GamePadButton::DPadDown, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(pCharacter_1.get(), 200.0f, glm::vec2{ 0 , 1 }));
	dae::InputManager::GetInstance().BindGamepadCommand(dae::Gamepad::GamePadButton::DPadLeft, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(pCharacter_1.get(), 200.0f, glm::vec2{ -1 , 0 }));
	dae::InputManager::GetInstance().BindGamepadCommand(dae::Gamepad::GamePadButton::DPadRight, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(pCharacter_1.get(), 200.0f, glm::vec2{ 1 , 0 }));

	auto pCharacter_2 = std::make_shared<dae::GameObject>();
	pCharacter_2->AddComponent<dae::RenderComponent>("test_2.png");
	pCharacter_2->AddComponent<dae::Transform>()->SetLocalPosition(300.f, 300.f, 0.f);

	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(pCharacter_2.get(), 200.0f , glm::vec2{0 , -1}));
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(pCharacter_2.get(), 200.0f, glm::vec2{ 0 , 1 }));
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(pCharacter_2.get(), 200.0f, glm::vec2{ -1 , 0 }));
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(pCharacter_2.get(), 200.0f, glm::vec2{ 1 , 0 }));

	//test for unbinding
	//dae::InputManager::GetInstance().UnbindKeyboardCommand(SDL_SCANCODE_W); 

	scene.Add(pCharacter_2);
	scene.Add(pCharacter_1);

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

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}