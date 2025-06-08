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
//
#include <filesystem>

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






	void RegularGameplayMode::SetupGameplayScene(dae::Scene& scene, int levelNumber, std::shared_ptr<TileMap>& outTileMap)
	{
		outTileMap = std::make_shared<TileMap>();
		dae::LevelLoader loader;

		switch (levelNumber)
		{
		case 1:
			loader.LoadLevel("../Data/level.txt", scene, *outTileMap, "../Data/Backgrounds_1.png");
			std::cout << "first level" << std::endl;
			break;
		case 2:
			loader.LoadLevel("../Data/level1.txt", scene, *outTileMap, "../Data/Backgrounds_2.png");
			std::cout << "second level" << std::endl;
			break;
		case 3:
			loader.LoadLevel("../Data/level2.txt", scene, *outTileMap, "../Data/Backgrounds_3.png");
			std::cout << "third level" << std::endl;
			break;
		default:
			std::cout << "No more levels. You win!\n";
			return;
		}


		int& sharedScore = m_Controller->GetMutableScore();

		auto pCharacter = std::make_shared<dae::GameObject>();

		auto playerComp = pCharacter->AddComponent<dae::PlayerComponent>(scene, outTileMap , sharedScore);
		playerComp->Initialize(glm::vec3{ TileMap::TILE_WIDTH * 7.f, TileMap::TILE_HEIGHT * 10.f, 3.f });
		playerComp->BindInput();
		m_pPlayerGameObject = pCharacter;
		scene.Add(pCharacter);

		/*const auto& spawnPoints = loader.GetEnemySpawnPositions();
		auto enemySpawnerGO = std::make_shared<dae::GameObject>();
		enemySpawnerGO->AddComponent<dae::EnemySpawner>(scene, outTileMap, spawnPoints, 1, 5.0f);
		enemySpawnerGO->AddComponent<dae::Transform>();
		scene.Add(enemySpawnerGO);*/
		
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
			std::cout << "[RegularGameplayMode] lifeIcon[" << i << "] RenderComponent: " << renderComp << "\n";
			lifeIcons.push_back(renderComp);
		}

		auto scoreComponent = pCharacter->GetComponent<dae::ScoreComponent>();
		pCharacter->GetComponent<dae::HealthComponent>();

		m_HUDObserver = pCharacter->AddComponent<dae::HUDObserver>(scoreText, scoreComponent, lifeIcons);

		dae::EventManager::GetInstance().AddObserver(m_HUDObserver, { EVENT_PLAYER_COLLECT_ITEM, EVENT_PLAYER_LOSING_LIFE }); 
		dae::EventManager::GetInstance().AddObserver(this, { EVENT_COLLECTED_ALL_GEMS });
	}

	void RegularGameplayMode::OnEnter()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Gameplay");

		SetupGameplayScene(scene, m_CurrentLevel, m_TileMap);
	}

	void RegularGameplayMode::Update(float /*deltaTime*/)
	{

	}

	void RegularGameplayMode::OnNotify(const dae::GameObject&, dae::Event event)
	{
		if (event != EVENT_COLLECTED_ALL_GEMS)
			return;

		if (m_CurrentLevel < 3)
		{
			DeferReloadScene(m_CurrentLevel + 1);
		}
		else
		{
			if (m_pPlayerGameObject && !m_GameCompletedFired)
			{
				EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
				m_GameCompletedFired = true;
			}
			return;
		}
	}

	void RegularGameplayMode::DeferReloadScene(int nextLevel)
	{
		m_NextLevel = nextLevel;
		m_ShouldReloadScene = true;
	}

	void RegularGameplayMode::ProcessDeferredReload()
	{
		if (!m_ShouldReloadScene)
			return;

		ClearSceneReferences();

		m_CurrentLevel = m_NextLevel;

		dae::SceneManager::GetInstance().DeleteScene("Gameplay");
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Gameplay");
		SetupGameplayScene(scene, m_CurrentLevel, m_TileMap);

		m_ShouldReloadScene = false;
	}

	void RegularGameplayMode::ClearSceneReferences()
	{
		if (m_pPlayerGameObject)
		{
			auto playerComp = m_pPlayerGameObject->GetComponent<PlayerComponent>();
			if (playerComp)
				playerComp->UnbindInput();
			m_pPlayerGameObject.reset();
		}

		if (m_HUDObserver)
		{
			dae::EventManager::GetInstance().RemoveObserver(m_HUDObserver);
			m_HUDObserver = nullptr;
		}

		m_TileMap.reset();
	}

	void RegularGameplayMode::OnExit()
	{
		ClearSceneReferences();
		dae::SceneManager::GetInstance().DeleteScene("Gameplay");
	}






	void EndScreenState::OnEnter()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("EndScreen");

		int finalScore = m_Controller->GetScore();

		auto font = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 28);

		auto scoreTextObj = std::make_shared<dae::GameObject>();

		std::ostringstream ss;
		ss << std::setfill('0') << std::setw(6) << finalScore;
		std::string scoreStr = ss.str();

		scoreTextObj->AddComponent<dae::TextComponent>(scoreStr, font);

		scoreTextObj->AddComponent<dae::Transform>()->SetLocalPosition(10.f, 1.f, 0.f);

		scene.Add(scoreTextObj);

		//initials
		float startX = 10.f;        
		float startY = 50.f;        
		float spacing = 30.f;       

		for (int i = 0; i < 3; ++i)
		{
			m_InitialTextObjects[i] = std::make_shared<dae::GameObject>();
			std::string letterStr(1, m_Initials[i]);

			m_InitialTextObjects[i]->AddComponent<dae::TextComponent>(letterStr, font);
			auto transform = m_InitialTextObjects[i]->AddComponent<dae::Transform>();
			transform->SetLocalPosition(startX + i * spacing, startY, 0.f);

			scene.Add(m_InitialTextObjects[i]);
		}
	}
	void EndScreenState::Update(float /*deltaTime*/)
	{
		

	}

	void EndScreenState::OnExit()
	{
		dae::SceneManager::GetInstance().DeleteScene("EndScreen");
	}

	//void EndScreenState::ChangeCurrentInitial(int /*delta*/)
	//{
	//	
	//}
}