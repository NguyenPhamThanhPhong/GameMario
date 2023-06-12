#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

#define PLANT_STATE_TOPLEFT		1
#define PLANT_STATE_TOPRIGHT	2
#define PLANT_STATE_BOTLEFT		3
#define PLANT_STATE_BOTRIGHT	4

#define PLANT_SPRITE_TOPLEFT 300100
#define PLANT_SPRITE_TOPRIGHT 300200
#define PLANT_SPRITE_BOTLEFT 300300
#define PLANT_SPRITE_BOTRIGHT 300400

#define PLANT_MOVE_RANGE 16


class CPlant : public CGameObject
{
private:
	float width;
	float height;
	ULONGLONG StopTime;
	bool IsStop;
	float TopMost;
	float BotMost;
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CPlant(float x, float y, float width, float height, float topmost, float botmost);
	virtual void SetState(int state);
};

