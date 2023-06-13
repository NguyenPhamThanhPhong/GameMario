#pragma once
#include <cstdlib>

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Game.h"
#include "FireBall.h"

#include "Mario.h"
#include "PlayScene.h"

#define PLANT_STATE_TOPLEFT		1
#define PLANT_STATE_TOPRIGHT	2
#define PLANT_STATE_BOTLEFT		3
#define PLANT_STATE_BOTRIGHT	4

#define PLANT_SPRITE_TOPLEFT 300100
#define PLANT_SPRITE_TOPRIGHT 300200
#define PLANT_SPRITE_BOTLEFT 300300
#define PLANT_SPRITE_BOTRIGHT 300400

#define PLANT_STATE_WAKEUP 5
#define PLANT_STATE_SLEEP	6
#define PLANT_STOP 7

#define PLANT_STOP_TIMEOUT 1000


class CPlant : public CGameObject
{
private:
	float width;
	float height;
	float TopMost;
	float BotMost;
	int current_sprite;
	ULONGLONG stop_start;
	bool IsShooting;
	bool IsinShootRange(float xx, float yy) {
		// y < -2.3*|x|
		float x_new = xx - x;
		float y_new = yy - TopMost;
		if (/*y_new>1.4*abs(x_new)*/ abs(x_new)<30) {
			return false;
		}
		return true;
	}
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x - width / 2;
		top = y - height / 2;
		right = x + width;
		bottom = y + height;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

		if (state == PLANT_STATE_WAKEUP) {
			if (y <= TopMost) {
				SetState(PLANT_STOP);
			}
			else if (y >= BotMost) {
				SetState(PLANT_STOP);
			}
		}
		else if (state == PLANT_STOP && stop_start!=-1) {
			if (GetTickCount64() - stop_start > PLANT_STOP_TIMEOUT) {
				SetState(PLANT_STATE_WAKEUP);
			}
		}
		else if (state == PLANT_STATE_SLEEP) {
			if (y >= BotMost)
				vy = 0;
		}
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		int xxx = mario->Getx();
		int yyy = mario->Gety();
		if (IsinShootRange(xxx, yyy)==true) {
			if(state== PLANT_STATE_SLEEP)
				SetState(PLANT_STATE_WAKEUP);
		}
		else
			SetState(PLANT_STATE_SLEEP);

		CFireball* fireball = (CFireball*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetFireball();

		if (IsShooting) {
			if (xxx < x) {
				if (abs(yyy - TopMost) < 8 && yyy<TopMost) {
					fireball->SetState(FIREBALL_STATE_TOPMID_LEFT);
				}
				else if (abs(yyy - TopMost) <8 && yyy > TopMost) {
					fireball->SetState(FIREBALL_STATE_BOTMID_LEFT);

				}
				else if (abs(yyy - TopMost) >= 8 && yyy < TopMost) {
					fireball->SetState(FIREBALL_STATE_TOPMOST_LEFT);

				}
				else if (abs(yyy - TopMost) >= 8 && yyy > TopMost) {
					fireball->SetState(FIREBALL_STATE_BOTMOST_LEFT);

				}
			}
			else if (xxx > x) {
				if (abs(yyy - TopMost) < 8 && yyy < TopMost) {
					fireball->SetState(FIREBALL_STATE_TOPMID_RIGHT);
				}
				else if (abs(yyy - TopMost) < 8 && yyy > TopMost) {
					fireball->SetState(FIREBALL_STATE_BOTMID_RIGHT);

				}
				else if (abs(yyy - TopMost) >= 8 && yyy < TopMost) {
					fireball->SetState(FIREBALL_STATE_TOPMOST_RIGHT);

				}
				else if (abs(yyy - TopMost) >= 8 && yyy > TopMost) {
					fireball->SetState(FIREBALL_STATE_BOTMOST_RIGHT);

				}
			}
		}

		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	virtual void Render() {
		int RenderId = PLANT_SPRITE_TOPLEFT;
		CSprites* s = CSprites::GetInstance();
		s->Get(RenderId)->Draw(x, y);
	}
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) {
		if (y <= TopMost - 3){
			y += 3;
		}
		else if (y >= BotMost + 3) {
			y -= 3;
		}
		y += vy * dt;
	}

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
		if (e->obj->IsBlocking())return;
	}
public:
	CPlant(float x, float y, float width, float height, float topmost, float botmost) :CGameObject(x, y) {
		this->width = width;
		this->height = height;
		this->vy = -0.04f;
		this->vx = 0;
		state = PLANT_STATE_WAKEUP;
		this->TopMost = topmost;
		this->BotMost = botmost;
		stop_start = -1;
		IsShooting = false;
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
		case PLANT_STOP: {
			if (y <= TopMost)
				IsShooting = true;
			vy = 0;
			stop_start = GetTickCount64();
			break;
		}
		case PLANT_STATE_WAKEUP:
		{
			IsShooting = false;
			if (y <= TopMost)
				vy = 0.04f;
			else if (y >= BotMost)
				vy = -0.04f;
			break;
		}
		case PLANT_STATE_SLEEP: {
			IsShooting = false;
			if (x >= BotMost)
				vy = 0;
			else if (y <= TopMost)
				vy = 0.04f;
			break;
		}
		default:
			break;
		}

	}
};

