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
		AddState("Versus", std::make_unique<VersusGameplayMode>(this));
		
		SetInitialState("MainMenu");

		EventManager::GetInstance().AddObserver(this, { EVENT_GAME_COMPLETED });
		std::cout << "GAME COMPLETED added as observer at address: " << this << std::endl;
	}

	GameController::~GameController()
	{
		EventManager::GetInstance().RemoveObserver(this);
	}

	void GameController::Update(float deltaTime)
	{
		std::string nextState = CheckNextState();
		if (!nextState.empty())
		{
			SwitchState(nextState);
		}

		StateMachine::Update(deltaTime);

		auto currentState = GetCurrentState();

		if (auto gameplayState = dynamic_cast<RegularGameplayMode*>(currentState))
		{
			gameplayState->ProcessDeferredReload();
		}
		else if (auto coopState = dynamic_cast<CoopGameplayMode*>(currentState))
		{
			coopState->ProcessDeferredReload();
		}
		else if (auto endState = dynamic_cast<EndScreenState*>(currentState))
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