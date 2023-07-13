#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define MARIO_ICON_MOVE_LEFT 1
#define MARIO_ICON_MOVE_RIGHT 2
#define MARIO_ICON_MOVE_TOP	3
#define MARIO_ICON_MOVE_BOT 4

#define ICON_VLOC 0.2f

class CMarioIcon : public CGameObject
{
private:
	bool isAtStage = false;
	bool Left = false;
	bool Right = true;
	bool Top = false;
	bool Bot = false;
protected:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMarioIcon(float x, float y) : CGameObject(x,y) {
		vx = 0;
		vy = 0;
	}
	bool GetisAtStage() {
		return isAtStage;
	}
	virtual void SetState(int state);
};