#pragma once
#include <math.h>
#include "GameObject.h"
#include "Game.h"

#include "InvisibleBlock.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Tail.h"

#define FLYGOOMBA_GRAVITY 0.002f
#define FLYGOOMBA_WALKING_SPEED 0.05f


#define FLYGOOMBA_BBOX_WIDTH 16
#define FLYGOOMBA_BBOX_HEIGHT 14
#define FLYGOOMBA_BBOX_HEIGHT_DIE 7

#define FLYGOOMBA_DIE_TIMEOUT 200
#define FLYGOOMBA_FLY_TIMEOUT 1000

#define FLYGOOMBA_STATE_WALKING 100
#define FLYGOOMBA_STATE_DIE 200
#define FLYGOOMBA_STATE_FLY 300
#define FLYGOOMBA_STATE_FLYDOWN 400

#define ID_ANI_FLYGOOMBA_WALKING 505002
#define ID_ANI_FLYGOOMBA_DIE 505003
#define ID_ANI_FLYGOOMBA_FLY 505001

class CFlygoomba : public CGameObject
{
private:
	float leftmost;
	float rightmost;
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG fly_start = -1;
	ULONGLONG fly_end;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		if (state == FLYGOOMBA_STATE_DIE)
		{
			left = x - FLYGOOMBA_BBOX_WIDTH / 2;
			top = y - FLYGOOMBA_BBOX_HEIGHT_DIE / 2;
			right = left + FLYGOOMBA_BBOX_WIDTH;
			bottom = top + FLYGOOMBA_BBOX_HEIGHT_DIE;
		}
		else
		{
			left = x - FLYGOOMBA_BBOX_WIDTH / 2;
			top = y - FLYGOOMBA_BBOX_HEIGHT / 2;
			right = left + FLYGOOMBA_BBOX_WIDTH;
			bottom = top + FLYGOOMBA_BBOX_HEIGHT;
		}
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		vy += ay * dt;
		vx += ax * dt;

		if ((state == FLYGOOMBA_STATE_DIE) && (GetTickCount64() - die_start > FLYGOOMBA_DIE_TIMEOUT))
		{
			isDeleted = true;
			return;
		}
		if (state != FLYGOOMBA_STATE_DIE) {
			if (x <= leftmost) {
				vx = 0.05f;
			}
			else if (x >= rightmost) {
				vx = -0.05f;
			}
		}
		if (state == FLYGOOMBA_STATE_FLY && (GetTickCount64() - fly_start > 200)) {
			SetState(FLYGOOMBA_STATE_FLYDOWN);
		}
		else if (state == FLYGOOMBA_STATE_FLYDOWN && (GetTickCount64() - fly_end > 200)) {
			SetState(FLYGOOMBA_STATE_FLY);
		}

		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	virtual void Render() {
		int aniId = 505001;
		if (state == FLYGOOMBA_STATE_WALKING) {
			aniId = 505002;
		}
		else if (state == FLYGOOMBA_STATE_DIE)
			aniId = 505003;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) {
		x += vx * dt;
		y += vy * dt;
	}

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
		if (dynamic_cast<CTail*>(e->obj))
		{
			CTail* tail = dynamic_cast<CTail*>(e->obj);
			if (tail->GetState() == 2) {
				if (GetState() == FLYGOOMBA_STATE_FLY || GetState() == FLYGOOMBA_STATE_FLYDOWN) {
					SetState(FLYGOOMBA_STATE_WALKING);
				}
				else if (GetState() == FLYGOOMBA_STATE_WALKING) {
					SetState(FLYGOOMBA_STATE_DIE);
				}
			}
		}
		if (!e->obj->IsBlocking()) return;
		if (dynamic_cast<CFlygoomba*>(e->obj)) return;

		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
	}

public:
	CFlygoomba(float x, float y,float leftmost,float rightmost) :CGameObject(x, y) {
		this->ax = 0;
		this->ay = FLYGOOMBA_GRAVITY;
		die_start = -1;
		this->vx = -0.05f;
		SetState(FLYGOOMBA_STATE_FLY);
		fly_start = -1;
		fly_end = -1;
		this->leftmost = leftmost;
		this->rightmost = rightmost;
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
		case FLYGOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (FLYGOOMBA_BBOX_HEIGHT - FLYGOOMBA_BBOX_HEIGHT_DIE) / 2;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		case FLYGOOMBA_STATE_WALKING:
			ay = FLYGOOMBA_GRAVITY;
			break;
		case FLYGOOMBA_STATE_FLY:
			ay = -0.001f;
			fly_start = GetTickCount64();
			break;
		case FLYGOOMBA_STATE_FLYDOWN:
			ay = 0.002f;
			fly_end = GetTickCount64();
			break;
		}
	}
};