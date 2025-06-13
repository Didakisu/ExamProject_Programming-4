#pragma once
#include "Component.h"
#include <glm.hpp>
#include "Scene.h"
#include "TileMap.h"
#include "DirectionComponent.h"
#include "AnimationComponent.h"
#include "Transform.h"
#include "CollisionComponent.h"
#include "InputProfile.h"
#include "InputManager.h"
#include "GameCommands.h"

namespace dae
{
	class VersusEnemyComponent final : public Component
	{
	public:
		VersusEnemyComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap);
		~VersusEnemyComponent();
		
		void Initialize(const glm::vec3& startPosition);
		void Update(float deltaTime) override;
		void Die();

		void BindInput(const InputProfile& profile, const GamepadProfile& gpProfile);
		void UnbindInput();
		void Respawn();

		void SetEnraged(bool isEnraged);
		
	private:
		Scene& m_Scene;
		std::shared_ptr<TileMap> m_pTileMap;

		DirectionComponent* m_pDirectionComponent{};
		AnimationComponent* m_pAnimationComponent{};
		Transform* m_pTransform{};
		CollisionComponent* m_pCollisionComponent{};

		InputProfile m_InputProfile{};
		GamepadProfile m_GamepadProfile;

		bool m_InputsBound{ false };

		float m_Speed{ 50.f };
		glm::vec3 m_SpawnPosition{};
		bool m_IsDying{ false };

		float m_DeathTimer{};
		const float m_DeathDuration{ 0.5f };

		bool m_IsDead{false};
		bool m_ShouldRespawn{ false };
		float m_RespawnTimer{ 0.f };

		bool m_IsEnraged{ false };
		float m_EnragedTimer{ 0.f };
		float m_EnragedCooldown{ 10.f }; 

		float m_EnragedDuration{ 3.f };
		float m_CurrentEnragedTime{ 0.f };


		const float m_TimeBetweenEnrages = 10.f;
		const float m_EnrageDuration = 5.f;
	};
}