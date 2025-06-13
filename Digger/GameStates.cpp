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
#include <filesystem>
#include "EnemyComponent.h"
#include "VersusEnemyComponent.h"
#include "CherryPowerUp.h"
#include <ServiceLocator.h>


namespace dae
{
	//--MAIN MENU SCENE--
	void MainMenuState::OnEnter()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Main");
		m_pTextObject = std::make_shared<dae::GameObject>();
		auto font = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 30);
		m_pTextObject->AddComponent<dae::TextComponent>("D I G G E R", font);

		auto transform = m_pTextObject->AddComponent<dae::Transform>();
		transform->SetPosition(140.f, 10.f, 0.f);

		scene.Add(m_pTextObject);

		HighscoreManager hsManager("../Data/highscores.txt");
		hsManager.LoadHighscore(); 

		auto allScores = hsManager.GetAllScores();

		std::sort(allScores.begin(), allScores.end(), [](const auto& a, const auto& b) {
			return a.score > b.score;
			});

		int maxEntries = std::min(5, (int)allScores.size());
		float startY = 50.f;
		float lineHeight = 30.f;

		for (int i = 0; i < maxEntries; ++i)
		{
			std::ostringstream ss;
			ss << allScores[i].initials << "         " << std::setw(6) << std::setfill('0') << allScores[i].score;

			auto scoreTextObj = std::make_shared<dae::GameObject>();
			scoreTextObj->AddComponent<dae::TextComponent>(ss.str(), font);
			auto transformScores = scoreTextObj->AddComponent<dae::Transform>();
			transformScores->SetLocalPosition(90.f, startY + i * lineHeight, 0.f);

			scene.Add(scoreTextObj);
		}

		std::vector<std::string> modes{ "Regular", "Coop", "Versus" };
		float startX = 50.f;
		startY = m_IndicatorYStart;

		for (int i = 0; i < 3; ++i)
		{
			m_ModeTextObjects[i] = std::make_shared<dae::GameObject>();
			m_ModeTextObjects[i]->AddComponent<dae::TextComponent>(modes[i], font);
			m_ModeTextObjects[i]->AddComponent<dae::Transform>()->SetLocalPosition(startX, startY + i * m_IndicatorSpacing, 0.f);
			scene.Add(m_ModeTextObjects[i]);
		}

		m_SelectionIndicator = std::make_shared<dae::GameObject>();
		m_SelectionIndicator->AddComponent<dae::TextComponent>(">", font);
		m_SelectionIndicator->AddComponent<dae::Transform>()->SetLocalPosition(m_IndicatorX, startY + m_SelectedIndex * m_IndicatorSpacing, 0.f);
		scene.Add(m_SelectionIndicator);

		BindInput();
	}

	void MainMenuState::ChangeSelectedMode(int delta)
	{
		m_SelectedIndex = (3 + m_SelectedIndex + delta) % 3;

		if (auto transform = m_SelectionIndicator->GetComponent<dae::Transform>())
		{
			transform->SetLocalPosition(m_IndicatorX, m_IndicatorYStart + m_SelectedIndex * m_IndicatorSpacing, 0.f);
		}

		auto soundSystem = dae::ServiceLocator::GetSoundSystem();
		soundSystem->Play(UI_CHOOSE_SOUND_ID, 30);
	}

	void MainMenuState::ConfirmSelectedMode()
	{
		switch (m_SelectedIndex)
		{
		case 0: m_Controller->RequestStateChange("Gameplay"); break;
		case 1: m_Controller->RequestStateChange("Coop"); break;
		case 2: m_Controller->RequestStateChange("Versus"); break;
		default: break;
		}
	}

	void MainMenuState::BindInput()
	{
		auto& input = InputManager::GetInstance();
		input.BindKeyboardCommand(SDL_SCANCODE_UP, InputState::Down,
			std::make_unique<MainMenuSelectCommand>(m_Controller, -1));
		input.BindKeyboardCommand(SDL_SCANCODE_DOWN, InputState::Down,
			std::make_unique<MainMenuSelectCommand>(m_Controller, +1));
		input.BindKeyboardCommand(SDL_SCANCODE_RETURN, InputState::Down,
			std::make_unique<MainMenuConfirmCommand>(m_Controller));

		using GPB = Gamepad::GamePadButton;
		input.BindGamepadCommand(0,GPB::DPadUp, InputState::Down,
			std::make_unique<MainMenuSelectCommand>(m_Controller, -1));
		input.BindGamepadCommand(0 ,GPB::DPadDown, InputState::Down,
			std::make_unique<MainMenuSelectCommand>(m_Controller, +1));
		input.BindGamepadCommand(0 ,GPB::A, InputState::Down,
			std::make_unique<MainMenuConfirmCommand>(m_Controller));
	}

	void MainMenuState::UnbindInput()
	{
		auto& input = InputManager::GetInstance();
		input.UnbindKeyboardCommand(SDL_SCANCODE_UP);
		input.UnbindKeyboardCommand(SDL_SCANCODE_DOWN);
		input.UnbindKeyboardCommand(SDL_SCANCODE_RETURN);

		using GPB = Gamepad::GamePadButton;
		input.UnbindGamepadCommand(0 ,GPB::DPadUp);
		input.UnbindGamepadCommand(0 ,GPB::DPadDown);
		input.UnbindGamepadCommand(0, GPB::A);
	}


	void MainMenuState::Update(float /*deltaTime*/)
	{

	}

	void MainMenuState::OnExit()
	{
		dae::SceneManager::GetInstance().DeleteScene("Main");
		UnbindInput();
	}





	//--REGULAR GAMEPLAY MODE--
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
			dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
			m_GameCompletedFired = true;
			std::cout << "No more levels. You win!\n";
			return;
		}


		int& sharedScore = m_Controller->GetMutableScore();
		int& sharedLives = m_Controller->GetMutableLives();

		auto pCharacter = std::make_shared<dae::GameObject>();

		auto playerComp = pCharacter->AddComponent<dae::PlayerComponent>(scene, outTileMap , sharedScore , sharedLives);
		playerComp->Initialize(glm::vec3{ TileMap::TILE_WIDTH * 7.f, TileMap::TILE_HEIGHT * 10.f, 3.f });

		InputProfile inputP1{SDL_SCANCODE_W,SDL_SCANCODE_S,SDL_SCANCODE_A,SDL_SCANCODE_D,SDL_SCANCODE_SPACE};
		GamepadProfile gamepadP1{0 ,Gamepad::GamePadButton::DPadUp,Gamepad::GamePadButton::DPadDown,Gamepad::GamePadButton::DPadLeft,Gamepad::GamePadButton::DPadRight,Gamepad::GamePadButton::A };

		playerComp->BindInput(inputP1, gamepadP1);
		m_pPlayerGameObject = pCharacter;
		scene.Add(pCharacter);

		const auto& spawnPoints = loader.GetEnemySpawnPositions();
		auto enemySpawnerGO = std::make_shared<dae::GameObject>();
		enemySpawnerGO->AddComponent<dae::EnemySpawner>(scene, outTileMap, spawnPoints, m_TotalNumberOfEnemies, m_TimeBetweenEnemySpawn);
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
			std::cout << "[RegularGameplayMode] lifeIcon[" << i << "] RenderComponent: " << renderComp << "\n";
			lifeIcons.push_back(renderComp);
		}

		auto scoreComponent = pCharacter->GetComponent<dae::ScoreComponent>();
		pCharacter->GetComponent<dae::HealthComponent>();

		m_HUDObserver = pCharacter->AddComponent<dae::HUDObserver>(scoreText, scoreComponent, lifeIcons);

		dae::EventManager::GetInstance().AddObserver(m_HUDObserver, { EVENT_GAME_SCORE_CHANGED, EVENT_PLAYER_LOSING_LIFE });
		dae::EventManager::GetInstance().AddObserver(this, { EVENT_GAME_GEM_COLLECTED });
		dae::EventManager::GetInstance().AddObserver(this, { EVENT_ALL_ENEMIES_KILLED });
	}

	void RegularGameplayMode::OnEnter()
	{
		m_GameCompletedFired = false;
		m_Controller->GetMutableScore() = 0;
		m_Controller->GetMutableLives() = 4;
		ClearSceneReferences(); 
		m_CurrentLevel = 1;

		//skip levels
		auto& input = InputManager::GetInstance();
		input.BindKeyboardCommand(SDL_SCANCODE_F1, InputState::Down, std::make_unique<SkipLevelCommand>(m_Controller));

		auto soundSystem = dae::ServiceLocator::GetSoundSystem();
		soundSystem->Play(GAMEPLAY_MUSIC_SOUND_ID, 5);

		auto& scene = dae::SceneManager::GetInstance().CreateScene("Gameplay");
		SetupGameplayScene(scene, m_CurrentLevel, m_TileMap);
	}

	void RegularGameplayMode::Update(float /*deltaTime*/)
	{

	}

	void RegularGameplayMode::OnNotify(const dae::GameObject&, dae::Event event)
	{
		if (event == EVENT_GAME_GEM_COLLECTED)
		{
			++m_TotalGemsCollected;

			if (m_TotalGemsCollected >= LevelLoader::GetTotalGemCount() && LevelLoader::GetTotalGemCount() > 0)
			{
				std::cout << "Collected all gems!" << std::endl;

				if (m_CurrentLevel < 3)
				{
					DeferReloadScene(m_CurrentLevel + 1);
				}
				else
				{
					if (m_pPlayerGameObject && !m_GameCompletedFired)
					{
						dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
						m_GameCompletedFired = true;
					}
				}
			}
		}
		else if (event == EVENT_ALL_ENEMIES_KILLED)
		{
			std::cout << "All enemies killed this round!" << std::endl;

			if (m_CurrentLevel < 3)
			{
				DeferReloadScene(m_CurrentLevel + 1);
			}
			else if (!m_GameCompletedFired)
			{
				dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
				m_GameCompletedFired = true;
			}
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
		{
			return;
		}

		ClearSceneReferences();
		dae::SceneManager::GetInstance().DeleteScene("Gameplay");

		m_CurrentLevel = m_NextLevel;

		auto& scene = dae::SceneManager::GetInstance().CreateScene("Gameplay");
		SetupGameplayScene(scene, m_CurrentLevel, m_TileMap);

		m_ShouldReloadScene = false;
	}

	void RegularGameplayMode::ClearSceneReferences()
	{
		m_TotalGemsCollected = 0; 

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

		dae::EventManager::GetInstance().RemoveObserver(this);
		m_TileMap.reset();
	}

	void RegularGameplayMode::OnExit()
	{
		ClearSceneReferences();
		m_TotalGemsCollected = 0;
		m_CurrentLevel = 1;

		auto soundSystem = dae::ServiceLocator::GetSoundSystem();
		soundSystem->Stop(GAMEPLAY_MUSIC_SOUND_ID);

		dae::SceneManager::GetInstance().DeleteScene("Gameplay");
	}



	//--END SCENE--
	void EndScreenState::OnEnter()
	{
		m_Initials[0] = 'A';
		m_Initials[1] = 'A';
		m_Initials[2] = 'A';

		m_CurrentInitialIndex = 0;
		m_InputFinished = false;

		auto& scene = dae::SceneManager::GetInstance().CreateScene("EndScreen");
		int finalScore = m_Controller->GetScore();
		auto font = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 28);
		auto scoreTextObj = std::make_shared<dae::GameObject>();
		std::ostringstream ss;
		ss << std::setfill('0') << std::setw(6) << finalScore;
		std::string scoreStr = ss.str();

		scoreTextObj->AddComponent<dae::TextComponent>(scoreStr, font);
		scoreTextObj->AddComponent<dae::Transform>()->SetLocalPosition(10.f, 1.f, 0.f);

		auto highscoreTextObj = std::make_shared<dae::GameObject>();
		highscoreTextObj->AddComponent<dae::TextComponent>("NEW HIGH SCORE", font);
		highscoreTextObj->AddComponent<dae::Transform>()->SetLocalPosition(105.f, 60.f, 0.f);

		auto enterInitialsText = std::make_shared<dae::GameObject>();
		font = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 18);
		enterInitialsText->AddComponent<dae::TextComponent>("ENTER YOUR INITIALS", font);
		enterInitialsText->AddComponent<dae::Transform>()->SetLocalPosition(120.f, 110.f, 0.f);

		scene.Add(highscoreTextObj);
		scene.Add(enterInitialsText);
		scene.Add(scoreTextObj);

		font = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 100);

		//initials
		float startX = 110.f;        
		float startY = 120.f;        
		float spacing = 90.f;       

		for (int i = 0; i < 3; ++i)
		{
			m_InitialTextObjects[i] = std::make_shared<dae::GameObject>();
			std::string letterStr(1, m_Initials[i]);

			m_InitialTextObjects[i]->AddComponent<dae::TextComponent>(letterStr, font);
			auto transform = m_InitialTextObjects[i]->AddComponent<dae::Transform>();
			transform->SetLocalPosition(startX + i * spacing, startY, 0.f);

			scene.Add(m_InitialTextObjects[i]);
		}

		m_HighscoreManager = std::make_unique<dae::HighscoreManager>("../Data/highscores.txt");

		//indicator
		m_InitialIndicator = std::make_shared<dae::GameObject>();
		m_InitialIndicator->AddComponent<dae::TextComponent>("^", font);
		auto indicatorTransform = m_InitialIndicator->AddComponent<dae::Transform>();

		auto pos = CalculateIndicatorPosition(m_CurrentInitialIndex);
		indicatorTransform->SetLocalPosition(pos.x, pos.y, pos.z);

		scene.Add(m_InitialIndicator);

		BindInput();
	}

	void EndScreenState::BindInput()
	{
		auto& input = InputManager::GetInstance();
		input.BindKeyboardCommand(SDL_SCANCODE_UP, InputState::Down,
			std::make_unique<ChangeInitialLetterCommand>(m_Controller, +1));
		input.BindKeyboardCommand(SDL_SCANCODE_DOWN, InputState::Down,
			std::make_unique<ChangeInitialLetterCommand>(m_Controller, -1));
		input.BindKeyboardCommand(SDL_SCANCODE_RIGHT, InputState::Down,
			std::make_unique<ConfirmInitialLetterCommand>(m_Controller));
		input.BindKeyboardCommand(SDL_SCANCODE_LEFT, InputState::Down,
			std::make_unique<UndoInitialCommand>(m_Controller));
	}

	void EndScreenState::UnbindInput()
	{
		auto& input = InputManager::GetInstance();
		input.UnbindKeyboardCommand(SDL_SCANCODE_UP);
		input.UnbindKeyboardCommand(SDL_SCANCODE_DOWN);
		input.UnbindKeyboardCommand(SDL_SCANCODE_RIGHT);
		input.UnbindKeyboardCommand(SDL_SCANCODE_LEFT);
	}

	
	void EndScreenState::Update(float /*deltaTime*/)
	{
	
	}

	void EndScreenState::OnExit()
	{
		dae::SceneManager::GetInstance().DeleteScene("EndScreen");
		UnbindInput();
	}

	void EndScreenState::ChangeCurrentInitial(int delta)
	{
		std::cout << "ConfirmCurrentInitial called\n";


		if (m_InputFinished)
		{
			std::cout << "input has finished!" << std::endl;
			return;
		}
	
		if (m_CurrentInitialIndex < 0 || m_CurrentInitialIndex >= 3)
		{
			return;
		}

		char& letter = m_Initials[m_CurrentInitialIndex];

		letter = static_cast<char>('A' + (26 + (letter - 'A') + delta) % 26);

		if (auto textComp = m_InitialTextObjects[m_CurrentInitialIndex]->GetComponent<dae::TextComponent>())
		{
			textComp->SetText(std::string(1, letter));
		}
	}

	void EndScreenState::ConfirmCurrentInitial()
	{
		if (m_CurrentInitialIndex < 2)
		{
			++m_CurrentInitialIndex;

			if (auto transform = m_InitialIndicator->GetComponent<dae::Transform>())
			{
				auto pos = CalculateIndicatorPosition(m_CurrentInitialIndex);
				transform->SetLocalPosition(pos.x, pos.y, pos.z);
			}

			auto soundSystem = dae::ServiceLocator::GetSoundSystem();
			soundSystem->Play(UI_CHOOSE_SOUND_ID, 30);
		}
		else
		{
			m_InputFinished = true;

			std::string newInitials(m_Initials, 3);
			int newScore = m_Controller->GetScore();

			if (!m_HighscoreManager)
			{
				return;
			}

			m_HighscoreManager->LoadHighscore();
			int currentHighscore = m_HighscoreManager->GetHighscore();

			if (newScore > currentHighscore)
			{
				m_HighscoreManager->SaveHighscore(newInitials, newScore);
			}
		}
	}

	void EndScreenState::UndoConfirmInitial()
	{
		if (m_InputFinished)
		{
			return;
		}

		if (m_CurrentInitialIndex > 0)
		{
			--m_CurrentInitialIndex;

			if (auto transform = m_InitialIndicator->GetComponent<dae::Transform>())
			{
				auto pos = CalculateIndicatorPosition(m_CurrentInitialIndex);
				transform->SetLocalPosition(pos.x, pos.y, pos.z);
			}
		}
	}

	glm::vec3 EndScreenState::CalculateIndicatorPosition(int index) const
	{
		return glm::vec3(m_IndicatorStartX + index * m_IndicatorSpacing, m_IndicatorStartY + m_IndicatorYOffset, 0.f);
	}





	//--COOP GAME MODE--
	void CoopGameplayMode::SetupCoopGameplayScene(dae::Scene& scene, int levelNumber, std::shared_ptr<TileMap>& outTileMap)
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
			dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
			m_GameCompletedFired = true;
			std::cout << "No more levels. You win!\n";
			return;
		}

		int& sharedScore = m_Controller->GetMutableScore();
		int& sharedLives = m_Controller->GetMutableLives();

		auto pCharacter_1 = std::make_shared<dae::GameObject>();
		auto playerComp1 = pCharacter_1->AddComponent<dae::PlayerComponent>(scene, outTileMap, sharedScore , sharedLives);
		playerComp1->Initialize(glm::vec3{ TileMap::TILE_WIDTH * 7.f, TileMap::TILE_HEIGHT * 10.f, 3.f });
		InputProfile inputP1{ SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_SPACE };
		GamepadProfile gamepadP1{1, Gamepad::GamePadButton::DPadUp,Gamepad::GamePadButton::DPadDown,Gamepad::GamePadButton::DPadLeft,Gamepad::GamePadButton::DPadRight,Gamepad::GamePadButton::A };
		playerComp1->BindInput(inputP1, gamepadP1);
		m_pPlayer1GameObject = pCharacter_1;
		scene.Add(pCharacter_1);

		auto pCharacter_2 = std::make_shared<dae::GameObject>();
		auto playerComp2 = pCharacter_2->AddComponent<dae::PlayerComponent>(scene, outTileMap, sharedScore , sharedLives);
		playerComp2->Initialize(glm::vec3{ TileMap::TILE_WIDTH * 7.f, TileMap::TILE_HEIGHT * 10.f, 3.f });
		InputProfile inputP2{ SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_F };
		GamepadProfile gamepadP2{0, Gamepad::GamePadButton::DPadUp,Gamepad::GamePadButton::DPadDown,Gamepad::GamePadButton::DPadLeft,Gamepad::GamePadButton::DPadRight,Gamepad::GamePadButton::A };
		playerComp2->BindInput(inputP2 , gamepadP2);
		m_pPlayer2GameObject = pCharacter_2;
		scene.Add(pCharacter_2);

		const auto& spawnPoints = loader.GetEnemySpawnPositions();
		auto enemySpawnerGO = std::make_shared<dae::GameObject>();
		enemySpawnerGO->AddComponent<dae::EnemySpawner>(scene, outTileMap, spawnPoints, m_TotalNumberOfEnemies, m_TimeBetweenEnemySpawn);
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

		auto scoreComponent = pCharacter_1->GetComponent<dae::ScoreComponent>();
		m_HUDObserver = pCharacter_1->AddComponent<dae::HUDObserver>(scoreText, scoreComponent, lifeIcons);
		
		dae::EventManager::GetInstance().AddObserver(m_HUDObserver, { EVENT_GAME_SCORE_CHANGED, EVENT_PLAYER_LOSING_LIFE });
		dae::EventManager::GetInstance().AddObserver(this, { EVENT_GAME_GEM_COLLECTED });
		dae::EventManager::GetInstance().AddObserver(this, { EVENT_ALL_ENEMIES_KILLED });
	}

	void CoopGameplayMode::OnEnter()
	{
		m_CurrentLevel = 1;
		m_ShouldReloadScene = false;
		m_GameCompletedFired = false;

		m_Controller->GetMutableScore() = 0;
		m_Controller->GetMutableLives() = 4;

		//skip levels
		auto& input = InputManager::GetInstance();
		input.BindKeyboardCommand(SDL_SCANCODE_F1, InputState::Down, std::make_unique<SkipLevelCommand>(m_Controller));

		auto soundSystem = dae::ServiceLocator::GetSoundSystem();
		soundSystem->Play(GAMEPLAY_MUSIC_SOUND_ID, 5);

		ClearSceneReferences();
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Coop");
		SetupCoopGameplayScene(scene, m_CurrentLevel, m_TileMap);
	}

	void CoopGameplayMode::DeferReloadScene(int nextLevel)
	{
		m_NextLevel = nextLevel;
		m_ShouldReloadScene = true;
	}

	void CoopGameplayMode::OnNotify(const dae::GameObject& /*gameObject*/, dae::Event event)
	{
		if (event == EVENT_GAME_GEM_COLLECTED)
		{
			++m_TotalGemsCollected;

			if (m_TotalGemsCollected >= LevelLoader::GetTotalGemCount() && LevelLoader::GetTotalGemCount() > 0)
			{
				std::cout << "Collected all gems!" << std::endl;

				if (m_CurrentLevel < 3)
				{
					DeferReloadScene(m_CurrentLevel + 1);
				}
				else
				{
					if ((m_pPlayer1GameObject && m_pPlayer2GameObject) && !m_GameCompletedFired)
					{
						dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
						m_GameCompletedFired = true;
					}
				}
			}
		}
		else if (event == EVENT_ALL_ENEMIES_KILLED)
		{
			std::cout << "All enemies killed this round!" << std::endl;

			if (m_CurrentLevel < 3)
			{
				DeferReloadScene(m_CurrentLevel + 1);
			}
			else if (!m_GameCompletedFired)
			{
				dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
				m_GameCompletedFired = true;
			}
		}
	}

	void CoopGameplayMode::ProcessDeferredReload()
	{
		if (!m_ShouldReloadScene)
		{
			return;
		}

		ClearSceneReferences();
		dae::SceneManager::GetInstance().DeleteScene("Coop");

		m_CurrentLevel = m_NextLevel;
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Coop");
		SetupCoopGameplayScene(scene, m_CurrentLevel, m_TileMap);

		m_ShouldReloadScene = false;
	}


	void CoopGameplayMode::Update(float /*deltaTime*/)
	{

	}

	void CoopGameplayMode::ClearSceneReferences()
	{
		m_TotalGemsCollected = 0;

		if (m_pPlayer1GameObject)
		{
			auto playerComp = m_pPlayer1GameObject->GetComponent<PlayerComponent>();
			if (playerComp)
				playerComp->UnbindInput();
			m_pPlayer1GameObject.reset();
		}

		if (m_pPlayer2GameObject)
		{
			auto playerComp = m_pPlayer2GameObject->GetComponent<PlayerComponent>();
			if (playerComp)
				playerComp->UnbindInput();
			m_pPlayer2GameObject.reset();
		}

		if (m_HUDObserver)
		{
			dae::EventManager::GetInstance().RemoveObserver(m_HUDObserver);
			m_HUDObserver = nullptr;
		}

		dae::EventManager::GetInstance().RemoveObserver(this);

		m_TileMap.reset();
	}


	void CoopGameplayMode::OnExit()
	{
		ClearSceneReferences();
		m_TotalGemsCollected = 0;
		m_CurrentLevel = 1;
		dae::SceneManager::GetInstance().DeleteScene("Coop");
	}





	//--VERSUS GAME MODE--

	void VersusGameplayMode::SetupVersusGameplayScene(dae::Scene& scene, int levelNumber, std::shared_ptr<TileMap>& outTileMap)
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
			dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
			m_GameCompletedFired = true;
			std::cout << "No more levels. You win!\n";
			return;
		}

		int& sharedScore = m_Controller->GetMutableScore();
		int& sharedLives = m_Controller->GetMutableLives();

		//player
		auto pCharacter = std::make_shared<dae::GameObject>();

		auto playerComp = pCharacter->AddComponent<dae::PlayerComponent>(scene, outTileMap, sharedScore, sharedLives);
		playerComp->Initialize(glm::vec3{ TileMap::TILE_WIDTH * 7.f, TileMap::TILE_HEIGHT * 10.f, 3.f });

		InputProfile inputP1{ SDL_SCANCODE_W,SDL_SCANCODE_S,SDL_SCANCODE_A,SDL_SCANCODE_D,SDL_SCANCODE_SPACE };
		GamepadProfile gamepadP1{1, Gamepad::GamePadButton::DPadUp,Gamepad::GamePadButton::DPadDown,Gamepad::GamePadButton::DPadLeft,Gamepad::GamePadButton::DPadRight,Gamepad::GamePadButton::A};

		playerComp->BindInput(inputP1 , gamepadP1);
		m_pPlayerGameObject = pCharacter;
		scene.Add(pCharacter);

		//enemies
		const auto& spawnPoints = loader.GetEnemySpawnPositions();
		auto enemySpawnerGO = std::make_shared<dae::GameObject>();
		enemySpawnerGO->AddComponent<dae::EnemySpawner>(scene, outTileMap, spawnPoints, m_TotalNumberOfEnemies, m_TimeBetweenEnemySpawn);
		enemySpawnerGO->AddComponent<dae::Transform>();
		scene.Add(enemySpawnerGO);

		//enemy
		auto pEnemy = std::make_shared<dae::GameObject>();
		auto enemyComponent = pEnemy->AddComponent<dae::VersusEnemyComponent>(scene, outTileMap);
		pEnemy->AddComponent<dae::Transform>();
		enemyComponent->Initialize(glm::vec3{ TileMap::TILE_WIDTH * 12.f, TileMap::TILE_HEIGHT * 2.f, 3.f });
		InputProfile inputP2{SDL_SCANCODE_UP,SDL_SCANCODE_DOWN,SDL_SCANCODE_LEFT,SDL_SCANCODE_RIGHT,};
		GamepadProfile gamepadP2{ 0 ,Gamepad::GamePadButton::DPadUp,Gamepad::GamePadButton::DPadDown,Gamepad::GamePadButton::DPadLeft,Gamepad::GamePadButton::DPadRight,Gamepad::GamePadButton::A };
		enemyComponent->BindInput(inputP2 , gamepadP2);
		m_pEnemyGameObject = pEnemy;
		scene.Add(pEnemy);

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

		dae::EventManager::GetInstance().AddObserver(m_HUDObserver, { EVENT_GAME_SCORE_CHANGED, EVENT_PLAYER_LOSING_LIFE });
		dae::EventManager::GetInstance().AddObserver(this, { EVENT_GAME_GEM_COLLECTED });
	}

	void VersusGameplayMode::OnEnter()
	{
		m_CurrentLevel = 1;
		m_ShouldReloadScene = false;
		m_GameCompletedFired = false;

		m_Controller->GetMutableScore() = 0;
		m_Controller->GetMutableLives() = 4;

		//skip levels
		auto& input = InputManager::GetInstance();
		input.BindKeyboardCommand(SDL_SCANCODE_F1, InputState::Down, std::make_unique<SkipLevelCommand>(m_Controller));

		auto soundSystem = dae::ServiceLocator::GetSoundSystem();
		soundSystem->Play(GAMEPLAY_MUSIC_SOUND_ID, 5);

		ClearSceneReferences();
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Versus");
		SetupVersusGameplayScene(scene, m_CurrentLevel, m_TileMap);
	}

	void VersusGameplayMode::DeferReloadScene(int nextLevel)
	{
		m_NextLevel = nextLevel;
		m_ShouldReloadScene = true;
	}

	void VersusGameplayMode::OnNotify(const dae::GameObject& /*gameObject*/, dae::Event event)
	{
		if (event == EVENT_GAME_GEM_COLLECTED)
		{
			++m_TotalGemsCollected;

			if (m_TotalGemsCollected >= LevelLoader::GetTotalGemCount() && LevelLoader::GetTotalGemCount() > 0)
			{
				std::cout << "Collected all gems!" << std::endl;

				if (m_CurrentLevel < 3)
				{
					DeferReloadScene(m_CurrentLevel + 1);
				}
				else
				{
					if (m_pPlayerGameObject && !m_GameCompletedFired)
					{
						dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
						m_GameCompletedFired = true;
					}
				}
			}
		}
		else if (event == EVENT_ALL_ENEMIES_KILLED)
		{
			std::cout << "All enemies killed this round!" << std::endl;

			if (m_CurrentLevel < 3)
			{
				DeferReloadScene(m_CurrentLevel + 1);
			}
			else if (!m_GameCompletedFired)
			{
				dae::EventManager::GetInstance().FireEvent({ EVENT_GAME_COMPLETED }, nullptr, nullptr);
				m_GameCompletedFired = true;
			}
		}
	}


	void VersusGameplayMode::ProcessDeferredReload()
	{
		if (!m_ShouldReloadScene)
		{
			return;
		}

		ClearSceneReferences();
		dae::SceneManager::GetInstance().DeleteScene("Versus");

		m_CurrentLevel = m_NextLevel;
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Versus");
		SetupVersusGameplayScene(scene, m_CurrentLevel, m_TileMap);

		m_ShouldReloadScene = false;
	}

	void VersusGameplayMode::Update(float /*deltaTime*/)
	{

	}


	void VersusGameplayMode::ClearSceneReferences()
	{
		m_TotalGemsCollected = 0;

		if (m_pPlayerGameObject)
		{
			auto playerComp = m_pPlayerGameObject->GetComponent<PlayerComponent>();
			if (playerComp)
			{
				playerComp->UnbindInput();
			}
			m_pPlayerGameObject.reset();
		}

		if (m_HUDObserver)
		{
			dae::EventManager::GetInstance().RemoveObserver(m_HUDObserver);
			m_HUDObserver = nullptr;
		}

		dae::EventManager::GetInstance().RemoveObserver(this);

		m_TileMap.reset();
	}

	void VersusGameplayMode::OnExit()
	{
		ClearSceneReferences();
		m_TotalGemsCollected = 0;
		m_CurrentLevel = 1;
		dae::SceneManager::GetInstance().DeleteScene("Versus");
	}
}