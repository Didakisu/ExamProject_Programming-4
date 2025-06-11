#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void FixedUpdate(float fixedTimeStep);
		void Render();
		float GetDeltaTime() const;
		void GetCurrentScene();
		void DeleteScene(const std::string& name);
		void DeleteScenes();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		float m_DeltaTime;
		float m_FixedTimeStep;
	};
}