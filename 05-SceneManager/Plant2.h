#pragma once
#include <cstdlib>

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define PLANT_STATE_TOPLEFT		1
#define PLANT_STATE_TOPRIGHT	2
#define PLANT_STATE_BOTLEFT		3
#define PLANT_STATE_BOTRIGHT	4

#define PLANT_SPRITE_TOPLEFT 300100
#define PLANT_SPRITE_TOPRIGHT 300200
#define PLANT_SPRITE_BOTLEFT 300300
#define PLANT_SPRITE_BOTRIGHT 300400

#define PLANT_STATE_WAKEUP 5
#define PLANT_STATE_SLEEP	6
#define PLANT_STOP 7

#define PLANT_STOP_TIMEOUT 1000