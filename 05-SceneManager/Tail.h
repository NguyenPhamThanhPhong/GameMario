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
		float width = 30;
		float height = 20;
		left = x - width / 2 ;
		top = y - height / 2 ;
		right = x + width ;
		bottom = y + height ;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {



		//if (x <= xxx-30) {
		//	vx = -0.5f;
		//}
		//else if (x >= xxx + 30) {
		//	vx = 0.5f;
		//}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	virtual void Render() {
	}

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		int xxx = mario->Getx();
		int yyy = mario->Gety();
		x = xxx;
		y = yyy;
	}

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
		if (state == TAIL_SLEEP) {
			return;
		}
		else {
			//if()
			//CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

			//// jump on top >> kill Goomba and deflect a bit 
			//if (goomba->GetState() != GOOMBA_STATE_DIE)
			//{
			//	goomba->SetState(GOOMBA_STATE_DIE);
			//}
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