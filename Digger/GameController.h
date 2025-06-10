#pragma once
#include "StateMachine.h"
#include "Scene.h"
#include <RenderComponent.h>
#include "GameCommands.h"
#include "InputManager.h"
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class GameStates;
	class StateMachine;

	class GameController final : public StateMachine , public Component , public Observer
	{
	public:
		explicit GameController(GameObject* owner);
		~GameController();
		void Update(float deltaTime) override;
		void OnNotify(const GameObject&, Event event) override;

		void SetScene(Scene* scene) { m_pScene = scene; }
		Scene* GetScene() const { return m_pScene; }

		void RequestStateChange(const std::string& newState);
		void BindInput();

		int GetScore() const { return m_TotalScore; }
		int& GetMutableScore() { return m_TotalScore; }

		int GetLives() const { return m_TotalLives; }
		int& GetMutableLives() { return m_TotalLives; }

		void RequestInitialChange(int delta);
		void RequestInitialConfirm();
		void UndoConfirmInitial();
	protected:
		std::string CheckNextState() override;
	private:
		Scene* m_pScene = nullptr;
		RenderComponent* m_pRenderComponent = nullptr;
		std::string m_PendingState;
		int m_TotalScore{ 0 };
		int m_TotalLives{ 0 };
	};
}