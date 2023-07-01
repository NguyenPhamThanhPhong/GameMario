#pragma once
#include "GameObject.h"

#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

#define TAIL_SLEEP 1
#define TAIL_TRIGGER 2

class CTail : public CGameObject
{
protected:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		float width = 15;
		float height = 5;
		left = x - width / 2 ;
		top = y - height / 2 ;
		right = x + width ;
		bottom = y + height ;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float xxx = mario->Getx();
		float yyy = mario->Gety();
		y = yyy+5;

		if (x <= xxx-5) {
			vx = 2.0f;
		}
		else if (x >= xxx + 5) {
			vx = -2.0f;
		}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	virtual void Render() {
		RenderBoundingBox();
	}

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) {
		x += vx;
		//x = xxx;
		//y = yyy;
	}

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
		if (state == TAIL_SLEEP) {
			return;
		}
		else {
			//if()

		}
	}

public:
	CTail(float x, float y) :CGameObject(x,y) {
		SetState(TAIL_SLEEP);
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
	}
};