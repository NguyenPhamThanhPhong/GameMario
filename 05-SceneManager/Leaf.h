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
	int Rewardid;
	ULONGLONG wakeup_start;
	ULONGLONG direct_start;
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
		if (state == LEAF_STATE_WAKEUP && (GetTickCount64() - wakeup_start > 1000)) {
			SetState(LEAF_STATE_MOVE);
		}
		if (state == LEAF_STATE_MOVE) {
			if (vx < 0 && (GetTickCount64() - direct_start > 240)) {
				vx = 0.2f;
				direct_start = GetTickCount64();
			}
			else if (vx > 0 && (GetTickCount64() - direct_start > 240)) {
				vx = -0.2f;
				direct_start = GetTickCount64();
			}
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
		if (state != LEAF_STATE_SLEEP) {
			int renderId = LEAF_FLYLEFT;
			if (vx < 0) {
				renderId = LEAF_FLYRIGHT;
			}
			CSprites* s = CSprites::GetInstance();
			s->Get(renderId)->Draw(x, y);
		}

	}

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
public:
	CLeaf(float x, float y, float width, float height,int Rewardid) :CGameObject(x, y) {
		this->width = width;
		this->height = height;
		this->ay = 0;
		wakeup_start = -1;
		this->Rewardid = Rewardid;
		SetState(LEAF_STATE_SLEEP);
	}
	virtual int GetRewardIndex() { 
		DebugOut(L">>> leaf reward id: %d >>> \n",this->Rewardid);
		return this->Rewardid; }
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
		case LEAF_STATE_WAKEUP: {
			wakeup_start = GetTickCount64();
			vy = -0.05f;
			DebugOut(L">>> leaf wake up >>> \n");
			break;
		}
		case LEAF_STATE_MOVE: {
			vy = 0.05f;
			vx = 0.02f;
			direct_start = GetTickCount64();
			break;
		}
		}
	}
};