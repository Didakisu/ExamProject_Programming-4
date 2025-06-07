#pragma once
#include "Observer.h"

using sound_id = int; 

//sounds
constexpr sound_id DEATH_SOUND_ID = 0;
//events
constexpr dae::Event EVENT_PLAYER_LOSING_LIFE{dae::USER_SPACE + 0 };
constexpr dae::Event EVENT_PLAYER_COLLECT_ITEM{dae::USER_SPACE + 1 };
constexpr dae::Event EVENT_ENEMY_DIED{ dae::USER_SPACE + 2 };



constexpr dae::Event EVENT_COLLECTED_GEM{ dae::USER_SPACE + 3 };
constexpr dae::Event EVENT_COLLECTED_GOLD{ dae::USER_SPACE + 4 };
constexpr dae::Event EVENT_PLAYER_GAINED_LIFE{ dae::USER_SPACE + 5 };


