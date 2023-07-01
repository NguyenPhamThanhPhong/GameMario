#pragma once
#include "GameObject.h"
#include <math.h>

#define MUSHROOM_GRAVITY 0.002f

#define LEAF_STATE_SLEEP 1
#define LEAF_STATE_WAKEUP 2
#define LEAF_STATE_MOVE 3


#define LEAF_FLYLEFT 162001
#define LEAF_FLYRIGHT 162002

class CLeaf :public CGameObject {
private:
	float width;
	float height;
	float ay;
	ULONGLONG wakeup_start;
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		if (state == LEAF_STATE_SLEEP) {
			left = x - width / 2;
			top = y - height / 2;
			right = x + width;
			bottom = y + height;
		}
		else {
			width = 12;
			height = 6;
			left = x - width / 2;
			top = y - height / 2;
			right = x + width;
			bottom = y + height;
		}

	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		vy += ay * dt;
		if (state == LEAF_STATE_WAKEUP && (GetTickCount64() - wakeup_start > 1500)) {
			SetState(LEAF_STATE_MOVE);
		}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	virtual void OnNoCollision(DWORD dt) {
		if (state == LEAF_STATE_WAKEUP)
		{
			x += vx * dt;
			y += vy * dt;
		}
		else if (state == LEAF_STATE_MOVE) {
			x += vx * dt;
			y += vy * dt;
		}
	}
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
	}
	virtual void Render() {
		int renderId = LEAF_FLYRIGHT;
		if (vx < 0) {
			renderId = LEAF_FLYLEFT;
		}
		CSprites* s = CSprites::GetInstance();
		s->Get(renderId)->Draw(x, y);
	}

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
public:
	CLeaf(float x, float y, float width, float height) :CGameObject(x, y) {
		this->width = width;
		this->height = height;
		this->ay = 0;
		wakeup_start = -1;
		SetState(LEAF_STATE_SLEEP);
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
		case LEAF_STATE_WAKEUP: {
			wakeup_start = GetTickCount64();
			vy = -0.05f;
			break;
		}
		case LEAF_STATE_MOVE: {
			vy = 0.05f;
			vx = 0;
			break;
		}
		}
	}
};