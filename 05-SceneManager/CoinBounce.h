#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN_BOUNCE 11000

#define	COIN_WIDTH_BOUNCE 10
#define COIN_BBOX_WIDTH_BOUNCE 10
#define COIN_BBOX_HEIGHT_BOUNCE 16

#define COIN_BOUNCE_LIVE 1
#define COIN_BOUNCE_DIE 2

#define COIN_DIE_TIMEOUT 500
class CCoinBounce : public CGameObject {
private:
	ULONGLONG die_start;
protected:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		if ((state == COIN_BOUNCE_DIE) && (GetTickCount64() - die_start > COIN_DIE_TIMEOUT))
		{
			isDeleted = true;
			return;
		}		
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
public:
	CCoinBounce(float x,float y):CGameObject() {
		SetState(COIN_BOUNCE_LIVE);
	}
	void Render() {
		if (state != COIN_BOUNCE_LIVE)
		{
			CAnimations* animations = CAnimations::GetInstance();
			animations->Get(ID_ANI_COIN_BOUNCE)->Render(x, y);
		}
	}
	void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
			case COIN_BOUNCE_DIE:
			{
				die_start = GetTickCount64();
				vy = -1;
				break;
			}
		}
	}
	virtual void OnNoCollision(DWORD dt) {
		if(state == COIN_BOUNCE_DIE)
			y += vy;
	}

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {

	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - 14 / 2;
		t = y - 16 / 2;
		r = l + 14;
		b = t + 16;
	}
	int IsBlocking() { return 0; }
};