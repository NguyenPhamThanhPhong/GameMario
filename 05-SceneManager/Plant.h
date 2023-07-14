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

#define PLANT_GREEN_SPRITE_TOPLEFT	300501
#define PLANT_GREEN_SPRITE_TOPRIGHT 300504
#define PLANT_GREEN_SPRITE_BOTLEFT	300502
#define PLANT_GREEN_SPRITE_BOTRIGHT 300503


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
	int id;
	ULONGLONG stop_start;
	bool IsShooting;
	bool IsinShootRange(float xx, float yy) {
		// y < -2.3*|x|
		float x_new = xx - x;
		float y_new = yy - TopMost;
		if (y_new>1.4*abs(x_new)) {
			return false;
		}
		return true;
	}
	void HandleShoot(float xxx,float yyy) {
		CFireball* fireball = (CFireball*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetFireball(id);
		if(fireball!=NULL)
		if (IsShooting) {
			if (xxx < x) {
				if (abs(yyy - TopMost) < 40 && yyy < TopMost) {
					fireball->SetState(FIREBALL_STATE_TOPMID_LEFT);
					if(id==1)
						current_sprite = PLANT_SPRITE_TOPLEFT;
					else
						current_sprite = PLANT_GREEN_SPRITE_TOPLEFT;
					//DebugOut(L"plant shoot 1 \n");
				}
				else if (abs(yyy - TopMost) < 40 && yyy > TopMost) {
					fireball->SetState(FIREBALL_STATE_BOTMID_LEFT);
					if(id==1)
						current_sprite = PLANT_SPRITE_BOTLEFT;
					else
						current_sprite = PLANT_GREEN_SPRITE_BOTLEFT;
					//DebugOut(L"plant shoot 2 \n");

				}
				else if (abs(yyy - TopMost) >= 40 && yyy < TopMost) {
					fireball->SetState(FIREBALL_STATE_TOPMOST_LEFT);
					if (id == 1)
						current_sprite = PLANT_SPRITE_TOPLEFT;
					else
						current_sprite = PLANT_GREEN_SPRITE_TOPLEFT;
					//DebugOut(L"plant shoot 3 \n");

				}
				else if (abs(yyy - TopMost) >= 40 && yyy > TopMost) {
					fireball->SetState(FIREBALL_STATE_BOTMOST_LEFT);
					if (id == 1)
						current_sprite = PLANT_SPRITE_BOTLEFT;
					else
						current_sprite = PLANT_GREEN_SPRITE_BOTLEFT;
					//DebugOut(L"plant shoot 4 \n");

				}
			}
			else if (xxx > x) {
				if (abs(yyy - TopMost) < 40 && yyy < TopMost) {
					fireball->SetState(FIREBALL_STATE_TOPMID_RIGHT);
					if (id == 1)
						current_sprite = PLANT_SPRITE_TOPRIGHT;
					else
						current_sprite = PLANT_GREEN_SPRITE_TOPRIGHT;
					//DebugOut(L"plant shoot 5 \n");

				}
				else if (abs(yyy - TopMost) < 40 && yyy > TopMost) {
					fireball->SetState(FIREBALL_STATE_BOTMID_RIGHT);
					if (id == 1)
						current_sprite = PLANT_SPRITE_BOTRIGHT;
					else
						current_sprite = PLANT_GREEN_SPRITE_BOTRIGHT;
					//DebugOut(L"plant shoot 6 \n");

				}
				else if (abs(yyy - TopMost) >= 40 && yyy < TopMost) {
					fireball->SetState(FIREBALL_STATE_TOPMOST_RIGHT);
					if (id == 1)
						current_sprite = PLANT_SPRITE_TOPRIGHT;
					else
						current_sprite = PLANT_GREEN_SPRITE_TOPRIGHT;
					//DebugOut(L"plant shoot 7 \n");

				}
				else if (abs(yyy - TopMost) >= 40 && yyy > TopMost) {
					fireball->SetState(FIREBALL_STATE_BOTMOST_RIGHT);
					if (id == 1)
						current_sprite = PLANT_SPRITE_BOTRIGHT;
					else
						current_sprite = PLANT_GREEN_SPRITE_BOTRIGHT;
					//DebugOut(L"plant shoot 8 \n");

				}
			}
		}
	}

protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x - width / 2+4;
		top = y - height / 2+4;
		right = x + width-4;
		bottom = y + height-4;
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

		float xxx = mario->Getx();
		float yyy = mario->Gety();
		if (IsinShootRange(xxx, yyy)==true) {
			if(state== PLANT_STATE_SLEEP)
				SetState(PLANT_STATE_WAKEUP);
		}
		else
			SetState(PLANT_STATE_SLEEP);
		if (id > 0)
		{
			HandleShoot(xxx, yyy);
		}
		else
		{
			if (y<=TopMost)
				current_sprite = 300505;
			else
				current_sprite = 300506;
		}

		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}

	virtual void Render() {
		if (current_sprite != NULL) {
			CSprites* s = CSprites::GetInstance();
			s->Get(current_sprite)->Draw(x, y);
		}

	}
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) {
		if (y <= TopMost - 3){
			y += 3;
		}
		else if (y >= BotMost + 3) {
			y -= 3;
		}
		if (state == PLANT_STATE_WAKEUP) {
			if (vy == 0) {
				vy = -0.04f;
			}
		}
		if (state == PLANT_STATE_SLEEP) {
			if (vy < BotMost && vy == 0) {
				vy = 0.04f;
			}
		}
		y += vy * dt;
	}

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
		if (e->obj->IsBlocking())return;
	}
public:
	CPlant(float x, float y, float width, float height, float topmost, float botmost,int index) :CGameObject(x, y) {
		//width, height
		this->width = width;
		this->height = height;
		//veloc
		this->vy = -0.04f;
		this->vx = 0;
		//state
		state = PLANT_STATE_WAKEUP;
		//limits
		this->TopMost = topmost;
		this->BotMost = botmost;
		//timer, boolean
		stop_start = -1;
		IsShooting = false;
		current_sprite = PLANT_SPRITE_TOPLEFT;
		//index
		this->id = index;
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

