#pragma once
#include "State.h" 
#include <memory>
#include "TileMap.h"
#include <TextComponent.h>
#include "ResourceManager.h"
#include "Observer.h"
#include "Scene.h"
#include "HUDObserver.h"
#include "HighscoreManager.h"
#include <array>


namespace dae
{
	class GameController;

	class MainMenuState : public State
	{
	public:
		MainMenuState(GameController* controller) : m_Controller(controller) {}

		void OnEnter() override;
		void ChangeSelectedMode(int delta);
		void ConfirmSelectedMode();
		void BindInput();
		void UnbindInput();
		void Update(float deltaTime) override;
		void OnExit() override;
	private:
		GameController* m_Controller;
		std::shared_ptr<dae::GameObject> m_pTextObject{};

		int m_SelectedIndex{ 0 };
		std::array<std::shared_ptr<dae::GameObject>, 3> m_ModeTextObjects{};
		std::shared_ptr<dae::GameObject> m_SelectionIndicator{};
		float m_IndicatorX{ 20.f };
		float m_IndicatorYStart{ 220.f };
		float m_IndicatorSpacing{ 40.f };
	};



	class RegularGameplayMode : public State ,public Observer
	{
	public:
		RegularGameplayMode(GameController* controller) : m_Controller(controller) {}
		void SetupGameplayScene(dae::Scene& scene, int levelNumber, std::shared_ptr<TileMap>& outTileMap);

		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override;
		void OnNotify(const dae::GameObject& gameObject, dae::Event event) override;
		void DeferReloadScene(int nextLevel);
		void ProcessDeferredReload();
		void ClearSceneReferences();
	private:
		GameController* m_Controller;
		std::shared_ptr<TileMap> m_TileMap;

		int m_CurrentLevel{1};
		std::shared_ptr<dae::GameObject> m_pPlayerGameObject;
		HUDObserver* m_HUDObserver = nullptr;
		bool m_ShouldReloadScene{ false };
		int m_NextLevel = 0;
		bool m_GameCompletedFired{ false };

		int m_TotalGemsCollected{};
		int m_TotalNumberOfEnemies{ 7 };
		float m_TimeBetweenEnemySpawn{ 7.0 };
	};



	class EndScreenState : public State
	{
	public:
		EndScreenState(GameController* controller) : m_Controller(controller) {}

		void OnEnter() override;
		void BindInput();
		void UnbindInput();
		void Update(float deltaTime) override;
		void OnExit() override;
		void ChangeCurrentInitial(int delta);
		void ConfirmCurrentInitial();
		bool WantsToExit() const { return m_InputFinished; }
		void UndoConfirmInitial();
	private:
		GameController* m_Controller;
		
		int m_CurrentInitialIndex = 0; 
		char m_Initials[3] = { 'A', 'A', 'A' }; 
		std::shared_ptr<dae::GameObject> m_InitialTextObjects[3];
		bool m_InputFinished = false;

		std::unique_ptr<dae::HighscoreManager> m_HighscoreManager;
		std::shared_ptr<dae::GameObject> m_InitialIndicator{};
		glm::vec3 CalculateIndicatorPosition(int index) const;

		const float m_IndicatorStartX = 110.f;
		const float m_IndicatorStartY = 120.f;
		const float m_IndicatorSpacing = 90.f;
		const float m_IndicatorYOffset = 90.f;
	};



	class CoopGameplayMode final : public State , public Observer
	{
	public:
		CoopGameplayMode(GameController* controller) : m_Controller(controller) {}
		void OnEnter() override;
		void OnExit() override;
		void Update(float deltaTime) override;
		void ProcessDeferredReload();
		void ClearSceneReferences();
		void SetupCoopGameplayScene(dae::Scene& scene, int levelNumber, std::shared_ptr<TileMap>& outTileMap);
		void DeferReloadScene(int nextLevel);
		void OnNotify(const dae::GameObject& gameObject, dae::Event event) override;
	private:
		std::shared_ptr<dae::GameObject> m_pPlayer1GameObject{};
		std::shared_ptr<dae::GameObject> m_pPlayer2GameObject{};
		std::shared_ptr<TileMap> m_TileMap{};

		int m_CurrentLevel{ 1 };
		int m_NextLevel{};
		bool m_ShouldReloadScene{ false };
		bool m_GameCompletedFired{ false };

		HUDObserver* m_HUDObserver = nullptr;
		GameController* m_Controller{}; 

		int m_TotalGemsCollected{};

		int m_TotalNumberOfEnemies{ 7 };
		float m_TimeBetweenEnemySpawn{ 7.0 };
	};



	class VersusGameplayMode : public State, public Observer
	{
	public:
		VersusGameplayMode(GameController* controller) : m_Controller(controller) {}
		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override;
		void SetupVersusGameplayScene(dae::Scene& scene, int levelNumber, std::shared_ptr<TileMap>& outTileMap);
		void ClearSceneReferences();
		void ProcessDeferredReload();
		void DeferReloadScene(int nextLevel);
		void OnNotify(const dae::GameObject& gameObject, dae::Event event) override;
	private:
		std::shared_ptr<dae::GameObject> m_pPlayerGameObject;
		std::shared_ptr<dae::GameObject> m_pEnemyGameObject;
		HUDObserver* m_HUDObserver = nullptr;
		GameController* m_Controller{};
		std::shared_ptr<TileMap> m_TileMap{};

		int m_CurrentLevel{ 1 };
		int m_NextLevel{};
		bool m_ShouldReloadScene{ false };
		bool m_GameCompletedFired{ false };

		int m_TotalGemsCollected{};
	};
}
