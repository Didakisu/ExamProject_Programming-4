#include "Scene.h"
#include "GameObject.h"
#include <iostream>
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	// Iterate through game objects
	for (auto it = m_objects.begin(); it != m_objects.end();)
	{
		auto& gameObject = *it;

		// Remove objects marked for destruction
		if (gameObject->IsMarkedForDestruction())
		{
			it = m_objects.erase(it);  // Remove from the scene
			std::cout << "GameObject removed from scene.\n";
		}
		else
		{
			++it;
		}
	}


}

void dae::Scene::FixedUpdate()
{
	for (auto& pObject : m_objects)
	{
		pObject->FixedUpdate();  
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

