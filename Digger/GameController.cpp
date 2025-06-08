#include "GameController.h"
#include "GameStates.h"
#include "EventManager.h"
#include "Data.h"


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

		EventManager::GetInstance().AddObserver(this, { EVENT_GAME_COMPLETED });
		std::cout << "GAME COMPLETED added as observer at address: " << this << std::endl;
	}

	GameController::~GameController()
	{
		std::cout << "[GameController] Destructor called.\n";
		EventManager::GetInstance().RemoveObserver(this);
		std::cout << "GameController destructor called, observer removed.\n";
	}


	void GameController::BindInput()
	{
		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_P, InputState::Pressed,
			std::make_unique<RequestStateChangeCommand>(this, "Gameplay"));

		/*InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_UP, InputState::Pressed,
			std::make_unique<ChangeInitialLetterCommand>(this, +1));

		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_DOWN, InputState::Pressed,
			std::make_unique<ChangeInitialLetterCommand>(this, -1));*/
	}

	void GameController::Update(float deltaTime)
	{
		StateMachine::Update(deltaTime);

		if (auto gameplayState = dynamic_cast<RegularGameplayMode*>(GetCurrentState()))
		{
			gameplayState->ProcessDeferredReload();
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
		std::cout << "[GameController] OnNotify called!!!" << "\n";
		if (event == EVENT_GAME_COMPLETED)
		{
			std::cout << "[GameController] OnNotify received event: " << static_cast<int>(event) << "\n";
			std::cout << "[GameController] this = " << this << "\n";
			RequestStateChange("EndScreen");
		}
	}

	/*void GameController::RequestInitialChange(int delta)
	{
		auto state = dynamic_cast<EndScreenState*>(GetCurrentState());
		if (state)
		{
			state->ChangeCurrentInitial(delta);
		}
	}*/
}