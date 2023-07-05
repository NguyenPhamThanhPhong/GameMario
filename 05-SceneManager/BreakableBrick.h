#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BREAKABLE_BRICK 100092
#define ID_ANI_BREAKABLE_BRICK_DIE 100093

#define BREAKABLE_BRICK_LIVE 1
#define BREAKABLE_BRICK_SLEEP 2
#define BREAKABLE_BRICK_DIE 3

class CBreakableBrick : public CGameObject {
	int blocking_state=true;
	ULONGLONG die_start = -1;
public:
	CBreakableBrick(float x, float y) : CGameObject(x, y) {
		blocking_state = true;
		state = BREAKABLE_BRICK_LIVE;
	}
	virtual int IsBlocking() { return blocking_state; }
	void Render() {
		if (this == NULL)
			return;
		CAnimations* animations = CAnimations::GetInstance();
		if (state == BREAKABLE_BRICK_LIVE) {
			animations->Get(ID_ANI_BREAKABLE_BRICK)->Render(x, y);
		}
		else if (state == BREAKABLE_BRICK_DIE) {
			animations->Get(ID_ANI_BREAKABLE_BRICK_DIE)->Render(x, y);
		}
		RenderBoundingBox();
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		if (state == BREAKABLE_BRICK_DIE && GetTickCount64() - die_start > 150) {
			isDeleted = true;
			DebugOut(L">>> kill breakable brick >>> \n");
			return;
		}
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - 14 / 2;
		t = y - 16 / 2;
		r = l + 14;
		b = t + 16;
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		if (state == BREAKABLE_BRICK_DIE) {
			blocking_state = false;
			die_start = GetTickCount64();
		}
	}
};