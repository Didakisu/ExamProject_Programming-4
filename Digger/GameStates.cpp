#include "GameStates.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "LevelLoader.h"
#include "ScoreComponent.h"
#include "glm.hpp"
#include "HUDObserver.h"
#include "Data.h"
#include "EnemySpawner.h"


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

		//character
		auto pCharacter = std::make_shared<dae::GameObject>();
		auto playerComp = pCharacter->AddComponent<dae::PlayerComponent>(scene, m_TileMap);
		playerComp->Initialize(glm::vec3{ TileMap::TILE_WIDTH * 7.f, TileMap::TILE_HEIGHT* 10.f, 3.f });
		playerComp->BindInput();
		scene.Add(pCharacter);

		//enemies
		const auto& spawnPoints = loader.GetEnemySpawnPositions();
		auto enemySpawnerGO = std::make_shared<dae::GameObject>();
		enemySpawnerGO->AddComponent<dae::EnemySpawner>(scene, m_TileMap, spawnPoints, 1, 5.0f);
		enemySpawnerGO->AddComponent<dae::Transform>();
		scene.Add(enemySpawnerGO);

		auto font = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 28);

		auto scoreTextObj = std::make_shared<dae::GameObject>();
		auto scoreText = scoreTextObj->AddComponent<dae::TextComponent>("000000", font);
		scoreTextObj->AddComponent<dae::Transform>()->SetLocalPosition(10.f, 1.f, 0.f);
		scene.Add(scoreTextObj);

		std::vector<dae::RenderComponent*> lifeIcons;
		const float baseX = 150.f;
		const float baseY = 1.f;  
		const float spacing = 40.f; 

		for (int i = 0; i < 6; ++i)
		{
			auto lifeIconGO = std::make_shared<dae::GameObject>();
			auto renderComp = lifeIconGO->AddComponent<dae::RenderComponent>("Lives.png", 32, 32);
			lifeIconGO->AddComponent<dae::Transform>()->SetLocalPosition(baseX + spacing * i, baseY, 0.f);
			scene.Add(lifeIconGO);

			lifeIcons.push_back(renderComp);
		}

		auto scoreComponent = pCharacter->GetComponent<dae::ScoreComponent>();
		pCharacter->GetComponent<dae::HealthComponent>();

		auto hudObserver = pCharacter->AddComponent<dae::HUDObserver>(scoreText, scoreComponent, lifeIcons);

		dae::EventManager::GetInstance().AddObserver(hudObserver, std::vector<dae::Event>{EVENT_PLAYER_COLLECT_ITEM, EVENT_PLAYER_LOSING_LIFE});
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