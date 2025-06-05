#pragma once
#include "State.h" 
#include <memory>
#include "TileMap.h"
#include <TextComponent.h>
#include "ResourceManager.h"


namespace dae
{
	class GameController;

	class MainMenuState : public State
	{
	public:
		MainMenuState(GameController* controller) : m_Controller(controller) {}

		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override;
	private:
		GameController* m_Controller;
		std::shared_ptr<dae::GameObject> m_pTextObject{};
	};

	class RegularGameplayMode : public State
	{
	public:
		RegularGameplayMode(GameController* controller) : m_Controller(controller) {}

		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override;
	private:
		GameController* m_Controller;
		std::shared_ptr<TileMap> m_TileMap;
	};

	class EndScreenState : public State
	{
	public:
		EndScreenState(GameController* controller) : m_Controller(controller) {}

		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override;
	private:
		GameController* m_Controller;
	};

	/*class VersusGameplayMode : public State
	{
	public:
		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override;
	private:
	};

	class CoopGameplayMode : public State
	{
	public:
		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override;
	private:
	};*/
}
