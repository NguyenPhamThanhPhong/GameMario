#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CBackgroundAni : public CGameObject {
private:
	int aniId;
public:
	CBackgroundAni(float x, float y, int aniId) : CGameObject(x, y) {
		this->aniId = aniId;
	}
	void Render() {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(aniId)->Render(x, y);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	int IsBlocking() { return 0; }
};
class CCard : public CGameObject {
private:
	ULONGLONG ani_start = -1;
	ULONGLONG die_start = -1;
	int reward=1;
public:
	CCard(float x, float y) : CGameObject(x, y) {
		SetState(1);
	}
	void Render() {
		CSprites* s = CSprites::GetInstance();
		switch (reward) {
		case 1:
			if (state == 2) {
				s->Get(199016)->Draw(x, y);
			}
			else
				s->Get(199013)->Draw(x, y);
			break;
		case 2:
			if (state == 2) {
				s->Get(199017)->Draw(x, y);
			}
			else
			s->Get(199014)->Draw(x, y);
			break;
		case 3:
			if (state == 2) {
				s->Get(199018)->Draw(x, y);
			}
			else
			s->Get(199015)->Draw(x, y);
			break;
		default:
			s->Get(199013)->Draw(x, y);
			break;
		}
		RenderBoundingBox();
		
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		if (state == 1) {
			if (reward == 1 && GetTickCount64() - ani_start > 50) {
				reward = 2;
				ani_start = GetTickCount64();
			}
			else if (reward == 2 && GetTickCount64() - ani_start > 50) {
				reward = 3;
				ani_start = GetTickCount64();
			}
			else if (reward == 3 && GetTickCount64() - ani_start > 50) {
				reward = 1;
				ani_start = GetTickCount64();
			}
		}
		else if (state == 2) {
			if ((GetTickCount64() - die_start > 2000)) {
				((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->SetGameWin(reward);
				isDeleted = true;
				return;
			}
		}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	int IsCollidable() { return 1; }
	void OnNoCollision(DWORD dt) {
		if (state == 2)
			y += vy;
	}

	void OnCollisionWith(LPCOLLISIONEVENT e) {

	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - 14 / 2;
		t = y - 16 / 2;
		r = l + 14;
		b = t + 16;
	}
	int IsBlocking() { return 0; }
	void SetState(int state) {
		CGameObject::SetState(state);
		switch (state)
		{
		case 1:
			reward = 1;
			ani_start = GetTickCount64();
			break;
		case 2:
			vy = -1.0f;
			die_start = GetTickCount64();
			break;
		default:
			break;
		}
	}
};