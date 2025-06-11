#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

void dae::SceneManager::Update(float deltaTime) 
{
	m_DeltaTime = deltaTime;  

	for (auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::FixedUpdate(float fixedTimeStep)
{

	m_FixedTimeStep = fixedTimeStep;

	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();   
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}


float dae::SceneManager::GetDeltaTime() const
{
	return m_DeltaTime;
}

void dae::SceneManager::GetCurrentScene()
{

}

void dae::SceneManager::DeleteScene(const std::string& name)
{
	std::erase_if(m_scenes, [name](std::shared_ptr<Scene>& ptr) {return ptr->GetName() == name;});
}

void dae::SceneManager::DeleteScenes()
{
	m_scenes.clear();
}
