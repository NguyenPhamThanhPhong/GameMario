#pragma once
#include "GameObject.h"

#define MUSHROOM_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f

#define MUSHROOM_STATE_SLEEP 1
#define MUSHROOM_STATE_WAKEUP 2
#define MUSHROOM_STATE_MOVE 3

#define MUSHROOM_WAKEUP_TIMEOUT 500

class CMushroom :public CGameObject {
private:
	float width;
	float height;
	float ay;
	int spriteId;
	ULONGLONG wakeup_start;
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		if (state == MUSHROOM_STATE_SLEEP) {
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
		if (state == MUSHROOM_STATE_WAKEUP && (GetTickCount64() - wakeup_start > MUSHROOM_WAKEUP_TIMEOUT)) {
			SetState(MUSHROOM_STATE_MOVE);
		}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	virtual void OnNoCollision(DWORD dt) {
		x += vx * dt;
		y += vy * dt;
	}
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
		if (state == MUSHROOM_STATE_MOVE) {
			if (!e->obj->IsBlocking()) return;
			if (e->ny != 0)
			{
				vy = 0;
			}
			else if (e->nx != 0)
			{
				vx = -vx;
			}
		}
	}
	virtual void Render() {
		if (state != MUSHROOM_STATE_SLEEP) {
			CSprites* s = CSprites::GetInstance();
			s->Get(spriteId)->Draw(x, y);
		}

	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
public:
	CMushroom(float x, float y, float width, float height, int spriteId) :CGameObject(x, y) {
		this->width = width;
		this->height = height;
		this->spriteId = spriteId;
		this->ay = 0;
		wakeup_start = -1;
		SetState(MUSHROOM_STATE_SLEEP);
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
		case MUSHROOM_STATE_WAKEUP: {
			wakeup_start = GetTickCount64();
			vy = -0.05f;
			break;
		}
		case MUSHROOM_STATE_MOVE: {
			vy = 0;
			vx = 0.05f;
			ay = MUSHROOM_GRAVITY;
			break;
		}
		}
	}
};