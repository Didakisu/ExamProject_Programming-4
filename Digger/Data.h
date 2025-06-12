#pragma once
#include "Observer.h"

using sound_id = int; 

//sounds
constexpr sound_id DEATH_SOUND_ID = 0;
constexpr sound_id COLLECT_SOUND_ID = 1;
constexpr sound_id BAG_BREAK_SOUND_ID = 2;
constexpr sound_id SHOOTING_FIREBALL_SOUND_ID = 3;
constexpr sound_id SHOOTING_HIT_ENEMY_SOUND_ID = 4;
constexpr sound_id GAMEPLAY_MUSIC_SOUND_ID = 5;
constexpr sound_id CHERRY_PICKUP_SOUND_ID = 6;
constexpr sound_id UI_CHOOSE_SOUND_ID = 7;
constexpr sound_id UI_SELECT_SOUND_ID = 8;
//events
constexpr dae::Event EVENT_PLAYER_LOSING_LIFE{dae::USER_SPACE + 0 };
//constexpr dae::Event EVENT_PLAYER_COLLECT_ITEM{dae::USER_SPACE + 1 };
constexpr dae::Event EVENT_ENEMY_DIED{ dae::USER_SPACE + 2 };


constexpr dae::Event EVENT_COLLECTED_GEM{ dae::USER_SPACE + 3 };
constexpr dae::Event EVENT_COLLECTED_GOLD{ dae::USER_SPACE + 4 };
constexpr dae::Event EVENT_PLAYER_GAINED_LIFE{ dae::USER_SPACE + 5 };

constexpr dae::Event EVENT_COLLECTED_ALL_GEMS{ dae::USER_SPACE + 6 };
constexpr dae::Event EVENT_GAME_COMPLETED{ dae::USER_SPACE + 7 };

constexpr dae::Event EVENT_GAME_SCORE_CHANGED{ dae::USER_SPACE + 8 };

constexpr dae::Event EVENT_GAME_GEM_COLLECTED{ dae::USER_SPACE + 9 };
constexpr dae::Event EVENT_CHERRY_COLLECTED{ dae::USER_SPACE + 10 };
constexpr dae::Event EVENT_ALL_ENEMIES_KILLED{ dae::USER_SPACE + 11 };
constexpr dae::Event EVENT_ENEMY_KILLED_BY_FIREBALL{ dae::USER_SPACE + 12 };

constexpr dae::Event EVENT_ENEMY_KILLED_BY_PLAYER_IN_BONUS_STATE{ dae::USER_SPACE + 13 };
constexpr dae::Event EVENT_PLAYER_OUT_OF_LIVES{ dae::USER_SPACE + 14 };