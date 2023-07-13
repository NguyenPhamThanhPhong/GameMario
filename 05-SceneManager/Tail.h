#pragma once
#include "GameObject.h"

#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"
#include "Tail.h"

#define TAIL_SLEEP 1
#define TAIL_TRIGGER 2

class CTail : public CGameObject
{
protected:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		float width = 7;
		float height = 5;
		left = x - width / 2 ;
		top = y - height / 2 ;
		right = x + width ;
		bottom = y + height ;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() {
		RenderBoundingBox();
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CTail(float x, float y) :CGameObject(x,y) {
		SetState(TAIL_SLEEP);
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
	}
};