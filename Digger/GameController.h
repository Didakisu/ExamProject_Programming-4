#pragma once
#include "StateMachine.h"
#include "Scene.h"
#include <RenderComponent.h>
#include "GameCommands.h"
#include "InputManager.h"
#include "Component.h"

namespace dae
{
	class GameStates;
	class StateMachine;

	class GameController final : public StateMachine , public Component
	{
	public:
		explicit GameController(GameObject* owner);
		void Update(float deltaTime) override;

		void SetScene(Scene* scene) { m_pScene = scene; }
		Scene* GetScene() const { return m_pScene; }

		void RequestStateChange(const std::string& newState);
		void BindInput();

		/*void SetEnemiesFrozen(bool frozen) { m_EnemiesFrozen = frozen; }
		bool AreEnemiesFrozen() const { return m_EnemiesFrozen; }*/

	protected:
		std::string CheckNextState() override;
	private:
		Scene* m_pScene = nullptr;
		RenderComponent* m_pRenderComponent = nullptr;
		std::string m_PendingState;

		/*bool m_EnemiesFrozen = false;*/
	};
}