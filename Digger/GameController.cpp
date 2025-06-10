#include "GameController.h"
#include "GameStates.h"
#include "EventManager.h"
#include "Data.h"
#include <iostream>


namespace dae
{
	GameController::GameController(GameObject* owner)
		: Component(owner)
	{
		AddState("MainMenu", std::make_unique<MainMenuState>(this));
		AddState("Gameplay", std::make_unique<RegularGameplayMode>(this));
		AddState("EndScreen", std::make_unique<EndScreenState>(this));
		AddState("Coop", std::make_unique<CoopGameplayMode>(this));
		
		//SetInitialState("MainMenu");
		SetInitialState("Coop");

		BindInput();

		EventManager::GetInstance().AddObserver(this, { EVENT_GAME_COMPLETED });
		std::cout << "GAME COMPLETED added as observer at address: " << this << std::endl;
	}

	GameController::~GameController()
	{
		EventManager::GetInstance().RemoveObserver(this);
	}


	void GameController::BindInput()
	{
		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_P, InputState::Pressed,
			std::make_unique<RequestStateChangeCommand>(this, "Gameplay"));

		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_UP, InputState::Down,
			std::make_unique<ChangeInitialLetterCommand>(this, +1));

		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_DOWN, InputState::Down,
			std::make_unique<ChangeInitialLetterCommand>(this, -1));

		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_RIGHT, InputState::Down,
			std::make_unique<ConfirmInitialLetterCommand>(this));

		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_LEFT, InputState::Down,
			std::make_unique<UndoInitialCommand>(this));

	}

	void GameController::Update(float deltaTime)
	{
		StateMachine::Update(deltaTime);

		if (auto gameplayState = dynamic_cast<RegularGameplayMode*>(GetCurrentState()))
		{
			gameplayState->ProcessDeferredReload();
		}

		if (auto gameplayState = dynamic_cast<CoopGameplayMode*>(GetCurrentState()))
		{
			gameplayState->ProcessDeferredReload();
		}

		if (auto endState = dynamic_cast<EndScreenState*>(GetCurrentState()))
		{
			if (endState->WantsToExit())
			{
				RequestStateChange("MainMenu");
			}
		}
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

	void GameController::OnNotify(const GameObject&, Event event)
	{
		if (event == EVENT_GAME_COMPLETED)
		{
			int finalScore = GetScore();

			HighscoreManager hsManager("../Data/highscores.txt");
			hsManager.LoadHighscore();

			if (finalScore > hsManager.GetHighscore())
			{
				RequestStateChange("EndScreen");
			}
			else
			{
				RequestStateChange("MainMenu");
			}
		}
	}

	void GameController::RequestInitialChange(int delta)
	{
		auto state = dynamic_cast<EndScreenState*>(GetCurrentState());
		if (state)
		{
			state->ChangeCurrentInitial(delta);
		}
	}

	void GameController::RequestInitialConfirm()
	{
		if (auto state = dynamic_cast<EndScreenState*>(GetCurrentState()))
		{
			state->ConfirmCurrentInitial();
		}
	}

	void GameController::UndoConfirmInitial()
	{
		if (auto endState = dynamic_cast<EndScreenState*>(GetCurrentState()))
		{
			endState->UndoConfirmInitial();
		}
	}

}