#pragma once
#include "GameObject.h"
#include "InvisibleBlock.h"
#include "Goomba.h"

#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"
#include "Brick.h"
#include "CoinBounce.h"
#include "Leaf.h"
#include "Tail.h"
#include "Mushroom.h"

#define TURTLE_LIVE		1
#define TURTLE_SLEEP	2
#define TURTLE_SPIN_LEFT	3
#define TURTLE_SPIN_RIGHT	4
#define TURTLE_SLEEP_HOLD	5
#define TURTLE_DIE		6


#define TURTLE_SLEEP_TIMEOUT 5000


class CTurtle : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG bounce_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		float width = 16;
		float height = 12;
		if (state != TURTLE_LIVE)
		{
			width = 16;
			height = 8;
			if (state == TURTLE_SLEEP) {
				height = 5;
			}
		}
		left = x - width / 2;
		top = y - height / 2;
		right = x + width;
		bottom = y + height;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		vy += ay * dt;
		vx += ax * dt;
		if ((state == TURTLE_SLEEP || state == TURTLE_SLEEP_HOLD) && (GetTickCount64() - die_start > TURTLE_SLEEP_TIMEOUT))
		{
			y -= 10;
			SetState(TURTLE_LIVE);
			die_start = 0;
		}
		if (state == TURTLE_SLEEP && vx!=0 && (GetTickCount64()-bounce_start > 600)) {
				vx = 0;
		}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	virtual void Render() {
		int aniId = 504001;
		if (state == TURTLE_LIVE) {
			if (vx > 0) {
				aniId = 504002;
			}
		}
		else if (state == TURTLE_SLEEP|| state== TURTLE_SLEEP_HOLD) {
			aniId = 504003;
		}
		else if (state == TURTLE_SPIN_LEFT || state == TURTLE_SPIN_RIGHT) {
			aniId = 504004;
		}

		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) {
		if (state != TURTLE_SLEEP_HOLD) {
			x += vx * dt;
			y += vy * dt;
		}
		else {
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->GetnX() < 0) {
				x = mario->Getx() - 10;
				y = mario->Gety();
			}
			else if (mario->GetnX() > 0) {
				x = mario->Getx() + 10;
				y = mario->Gety();
			}
		}

	}

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
		if (state == TURTLE_LIVE) {
			if (dynamic_cast<CTransparentblock*>(e->obj)) {
				vx = -vx;
			}
			else if (dynamic_cast<CTail*>(e->obj))
			{
				CTail* tail = dynamic_cast<CTail*>(e->obj);
				if (tail->GetState() == TAIL_TRIGGER) {
					SetState(TURTLE_SLEEP);
					return;
				}
			}
		}
		else {
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			if (dynamic_cast<CGoomba*>(e->obj)) {
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					goomba->SetState(GOOMBA_STATE_DIE);
				}
			}
			else
			if (dynamic_cast<CMystericBrick*>(e->obj)) {
				CMystericBrick* mysteric = dynamic_cast<CMystericBrick*>(e->obj);
					mysteric->SetState(MYSTERIC_STATE_DIE);
			}
			if (dynamic_cast<CLeaf*>(e->obj)&& mario->Getlevel()>=MARIO_LEVEL_BIG) {
				CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
				if(leaf->GetState()  == LEAF_STATE_SLEEP)
					leaf->SetState(LEAF_STATE_WAKEUP);
			}
			else if (dynamic_cast<CMushroom*>(e->obj) && mario->Getlevel() == MARIO_LEVEL_SMALL) {
				CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
				if (mushroom->GetState() == MUSHROOM_STATE_SLEEP)
					mushroom->SetState(MUSHROOM_STATE_WAKEUP);
			}
			else if (dynamic_cast<CTail*>(e->obj)) {
				CTail* tail = dynamic_cast<CTail*>(e->obj);
				if (tail->GetState() == TAIL_TRIGGER) {
					if (state == TURTLE_SLEEP)
					{
						vy = -0.5f;
						if (e->nx > 0) {
							vx = 0.02f;
							bounce_start = GetTickCount64();
						}
						else if (e->nx < 0) {
							vx = -0.02f;
							bounce_start = GetTickCount64();
						}
					}
				}
			}
		}

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

public:
	CTurtle(float x, float y) :CGameObject(x, y) {
		this->ax = 0;
		this->ay = 0.002f;
		vy = 0;
		vx = 0.02f;
		die_start = -1;
		bounce_start = -1;
		state = TURTLE_LIVE;
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
		case TURTLE_LIVE:
		{
			ay = 0.002f;
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->GetnX() > 0)
				vx = -0.02f;
			else
				vx = 0.02f;
			break;
		}
		case TURTLE_SLEEP: {
			die_start = GetTickCount64();
			ay = 0.002f;
			vy = 0;
			vx = 0;
			break;
		}
		case TURTLE_SPIN_LEFT: {
			y -= 5;
			vx = -0.2f;
			vy = 0;
			ay = 0.002f;
			break;
		}
		case TURTLE_SPIN_RIGHT: {
			y -= 5;
			vx = 0.2f;
			vy = 0;
			ay = 0.002f;
			break;
		}
		case TURTLE_DIE: {
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		}
		case TURTLE_SLEEP_HOLD: {
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		}
		}
	}
};