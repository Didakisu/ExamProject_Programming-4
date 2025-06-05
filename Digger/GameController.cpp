#include "GameController.h"
#include "GameStates.h"

namespace dae
{
	GameController::GameController(GameObject* owner)
		: Component(owner)
	{
		AddState("MainMenu", std::make_unique<MainMenuState>(this));
		AddState("Gameplay", std::make_unique<RegularGameplayMode>(this));
		AddState("EndScreen", std::make_unique<EndScreenState>(this));
		
		SetInitialState("MainMenu");

		 BindInput();
	}

	void GameController::BindInput()
	{
		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_P, InputState::Pressed,
			std::make_unique<RequestStateChangeCommand>(this, "Gameplay"));
	}

	void GameController::Update(float deltaTime)
	{
		StateMachine::Update(deltaTime);
	}

	std::string GameController::CheckNextState()
	{
		std::string nextState = m_PendingState;
		m_PendingState.clear();
		return nextState;
	}

	void GameController::RequestStateChange(const std::string& newState)
	{
		m_PendingState = newState;
	}

}