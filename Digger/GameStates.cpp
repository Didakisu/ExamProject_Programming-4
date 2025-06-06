#include "GameStates.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "LevelLoader.h"
#include "ScoreComponent.h"
#include "glm.hpp"


namespace dae
{
	void MainMenuState::OnEnter()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Main");

		m_pTextObject = std::make_shared<dae::GameObject>();

		auto font = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 30);
		m_pTextObject->AddComponent<dae::TextComponent>("D I G G E R", font);

		auto transform = m_pTextObject->AddComponent<dae::Transform>();
		transform->SetPosition(140.f, 10.f, 0.f);

		scene.Add(m_pTextObject);
	}

	void MainMenuState::Update(float /*deltaTime*/)
	{

	}

	void MainMenuState::OnExit()
	{
		dae::SceneManager::GetInstance().DeleteScene("Main");
	}


	void RegularGameplayMode::OnEnter()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Gameplay");

		m_TileMap = std::make_shared<TileMap>();

		dae::LevelLoader loader;
		loader.LoadLevel("../Data/level.txt", scene, *m_TileMap);

		auto pCharacter = std::make_shared<dae::GameObject>();
		auto playerComp = pCharacter->AddComponent<dae::PlayerComponent>(scene, m_TileMap);
		playerComp->Initialize(glm::vec3{ TileMap::TILE_WIDTH * 7.f, TileMap::TILE_HEIGHT* 10.f, 3.f });
		playerComp->BindInput();
		scene.Add(pCharacter);
	}

	void RegularGameplayMode::Update(float /*deltaTime*/)
	{

	}

	void RegularGameplayMode::OnExit()
	{
		dae::SceneManager::GetInstance().DeleteScene("Gameplay");
	}



	void EndScreenState::OnEnter()
	{

	}
	void EndScreenState::Update(float /*deltaTime*/)
	{

	}
	void EndScreenState::OnExit()
	{

	}
}