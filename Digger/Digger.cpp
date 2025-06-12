//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>

#endif
#endif

#include <SDL_scancode.h>
#include "Minigin.h"
#include "SceneManager.h"
#include "GameObject.h"  
#include "GameController.h"
#include <ServiceLocator.h>
#include "Data.h"


void load()
{
	auto& soundSystem = *dae::ServiceLocator::GetSoundSystem();
	soundSystem.LoadSound(DEATH_SOUND_ID, "../Data/death.wav");
	soundSystem.LoadSound(COLLECT_SOUND_ID, "../Data/collect.wav");
	soundSystem.LoadSound(BAG_BREAK_SOUND_ID, "../Data/Bag_Break.wav");
	soundSystem.LoadSound(SHOOTING_FIREBALL_SOUND_ID, "../Data/shooting_fireball.wav");
	soundSystem.LoadSound(SHOOTING_HIT_ENEMY_SOUND_ID, "../Data/Data_Sounds_ProjectileHitEntities.wav");
	soundSystem.LoadSound(GAMEPLAY_MUSIC_SOUND_ID, "../Data/Data_Sounds_DiggerMusic.mp3");
	soundSystem.LoadSound(CHERRY_PICKUP_SOUND_ID, "../Data/pickup_cherry.wav");
	soundSystem.LoadSound(UI_CHOOSE_SOUND_ID, "../Data/ui_choose.mp3");
	soundSystem.LoadSound(UI_SELECT_SOUND_ID, "../Data/ui_select.mp3");

	auto& rootScene = dae::SceneManager::GetInstance().CreateScene("RootScene");

	auto gameControllerGO = std::make_shared<dae::GameObject>();
	gameControllerGO->AddComponent<dae::GameController>();
	rootScene.Add(gameControllerGO);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}