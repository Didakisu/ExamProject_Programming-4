#pragma once
#include "State.h" 
#include "TileMap.h"

namespace dae
{
	class EnemyComponent;
	class GameObject;

	class EnemyNormalState : public State
	{
	public:
		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override;

		explicit EnemyNormalState(EnemyComponent* enemy) : m_Enemy(enemy) {}

	private:
		EnemyComponent* m_Enemy{};
	};

	class EnemyEnragedState : public State
	{
	public:
		void OnEnter() override;
		void Update(float deltaTime) override;

		explicit EnemyEnragedState(EnemyComponent* enemy) : m_Enemy(enemy){}
	private:
		EnemyComponent* m_Enemy{};
	};

	class EnemyDeadState : public State
	{
	public:
		void OnEnter() override;
		void Update(float deltaTime) override;

		explicit EnemyDeadState(EnemyComponent* enemy) : m_Enemy(enemy) {}
	private:
		EnemyComponent* m_Enemy{};
	};

	class EnemyBonusState : public State
	{
	public:
		void OnEnter() override;
		void Update(float deltaTime) override;
		void OnExit() override {}

		explicit EnemyBonusState(EnemyComponent* enemy) : m_Enemy(enemy) {}

	private:
		EnemyComponent* m_Enemy{};
		float m_BonusTimer{ 15.f }; 
		std::string m_OriginalDirtTexture;
	};

}

