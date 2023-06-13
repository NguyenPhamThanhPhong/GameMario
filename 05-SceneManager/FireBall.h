#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Game.h"

// X RANGE NOT BEING SHOT
#define CLOSEST_RANGE_LEFT	50
#define CLOSEST_RANGE_RIGHT	50

// Y RANGE NOT BEING SHOT

// not needed yet
#define FIREBALL_STATE_TOPMOST_LEFT		1
#define FIREBALL_STATE_TOPMID_LEFT		2
#define FIREBALL_STATE_BOTMID_LEFT		3
#define FIREBALL_STATE_BOTMOST_LEFT		4

#define FIREBALL_STATE_TOPMOST_RIGHT	5
#define FIREBALL_STATE_TOPMID_RIGHT		6
#define FIREBALL_STATE_BOTMID_RIGHT		7
#define FIREBALL_STATE_BOTMOST_RIGHT	8

#define FIREBALL_STATE_HIDDEN			9

// SPEED VALUE
#define FIREBALL_SPEED1	0.03f
#define FIREBALL_SPEED2	0.05f
#define FIREBALL_SPEED3	0.07f

class CFireball : public CGameObject {
private:
	float width;
	float height;
	int AniId;
	void resetxy() {
		x = 0;
		y = 0;
	}
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x - width / 2;
		top = y - height / 2;
		right = x + width;
		bottom = y + height;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	virtual void Render() {
		if (state != FIREBALL_STATE_HIDDEN) {
			CAnimations::GetInstance()->Get(AniId)->Render(x, y);
		}
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) {
		x += vx * dt;
		y += vy * dt;
	}

	virtual void OnCollisionWith(LPCOLLISIONEVENT e){}
public:
	CFireball(float x, float y, float width, float height,int aniId) : CGameObject(x,y) {
		this->width = width;
		this->height = height;
		this->AniId = aniId;
		state = FIREBALL_STATE_HIDDEN;
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
			case FIREBALL_STATE_TOPMOST_LEFT:
			{
				vx = -FIREBALL_SPEED1;
				vy = -FIREBALL_SPEED2;
				resetxy();
				break;
			}
			case FIREBALL_STATE_TOPMID_LEFT:
			{
				vx = -FIREBALL_SPEED2;
				vy = -FIREBALL_SPEED1;
				resetxy();
				break;
			}
			case FIREBALL_STATE_BOTMID_LEFT:
			{
				vx = -FIREBALL_SPEED2;
				vy = +FIREBALL_SPEED1;
				resetxy();
				break;
			}
			case FIREBALL_STATE_BOTMOST_LEFT:
			{
				vx = -FIREBALL_SPEED1;
				vy = +FIREBALL_SPEED2;
				resetxy();
				break;
			}
			case FIREBALL_STATE_TOPMOST_RIGHT:
			{
				vx = +FIREBALL_SPEED1;
				vy = -FIREBALL_SPEED2;
				resetxy();
				break;
			}
			case FIREBALL_STATE_TOPMID_RIGHT:
			{
				vx = +FIREBALL_SPEED2;
				vy = -FIREBALL_SPEED1;
				resetxy();
				break;
			}
			case FIREBALL_STATE_BOTMID_RIGHT:
			{
				vx = +FIREBALL_SPEED2;
				vy = +FIREBALL_SPEED1;
				resetxy();
				break;
			}
			case FIREBALL_STATE_BOTMOST_RIGHT:
			{
				vx = +FIREBALL_SPEED1;
				vy = +FIREBALL_SPEED2;
				resetxy();
				break;
			}
		}
	}
};