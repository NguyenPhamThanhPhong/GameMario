#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "CoinHidden.h"

#define ID_ANI_BREAKABLE_BRICK 92
#define ID_ANI_BREAKABLE_BRICK_DIE 93

#define BREAKABLE_BRICK_LIVE 1
#define BREAKABLE_BRICK_SLEEP 2
#define BREAKABLE_BRICK_DIE 3

class CBreakableBrick : public CGameObject {
	int blocking_state=true;
	int coinid;
	ULONGLONG die_start = -1;
	ULONGLONG sleep_start = -1;
	bool isCoinDeleted = false;
public:
	CBreakableBrick(float x, float y,int coinid) : CGameObject(x, y) {
		blocking_state = true;
		state = BREAKABLE_BRICK_LIVE;
		this->coinid = coinid;
	}
	virtual int IsBlocking() { return blocking_state; }
	void Render() {
		CAnimations* animations = CAnimations::GetInstance();
		if (state == BREAKABLE_BRICK_LIVE) {
			animations->Get(ID_ANI_BREAKABLE_BRICK)->Render(x, y);
		}
		else if (state == BREAKABLE_BRICK_DIE) {
			animations->Get(ID_ANI_BREAKABLE_BRICK_DIE)->Render(x, y);
		}
	}
	int IsControlledGroup() { return true; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		if (state == BREAKABLE_BRICK_DIE && GetTickCount64() - die_start > 150) {
			isDeleted = true;
			DebugOut(L">>> kill breakable brick >>> \n");
			return;
		}
		else if (state == BREAKABLE_BRICK_SLEEP && GetTickCount64() - sleep_start > 1500) {
			SetState(BREAKABLE_BRICK_LIVE);

		}
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - 14 / 2;
		t = y - 16 / 2;
		r = l + 14;
		b = t + 16;
	}
	void Killcoin() {
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->DeleteCoinHIdden(coinid);
		this->coinid = -1;
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
		case BREAKABLE_BRICK_DIE:
		{
			die_start = GetTickCount64();
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->DeleteCoinHIdden(coinid);
			this->coinid = -1;

			break;
		}
		case BREAKABLE_BRICK_SLEEP:
		{
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->TriggerCoinHIdden(coinid);
			blocking_state = false;
			sleep_start = GetTickCount64();
			break;
		}
		case BREAKABLE_BRICK_LIVE:
		{
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->DeleteCoinHIdden(coinid);
			coinid = -1;
			blocking_state = true;
			break;
		}
		}

	}

};