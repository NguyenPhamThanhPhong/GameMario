#pragma once
#include "Plant.h"
#include "Coin.h"

CPlant::CPlant(float x, float y,float width,float height,float topmost,float botmost) :CGameObject(x,y) {
	this->width = width;
	this->height = height;
	this->vy = -1;
	StopTime = 30;
	state = PLANT_STATE_TOPLEFT;
	IsStop = false;
	this->TopMost = topmost;
	this->BotMost = botmost;
}
void CPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - width / 2;
	top = y - height / 2;
	right = x + width;
	bottom = y + height;
}
void CPlant::OnNoCollision(DWORD dt) {
}
void CPlant::OnCollisionWith(LPCOLLISIONEVENT e) {

}
void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!IsStop) {
		y += vy;
	}
	int nextV = 1;
	if (y <= TopMost) {
		IsStop = true;
		vy = 0;
	}
	else if (y >= BotMost) {
		IsStop = true;
		vy = 0;
		nextV = -1;
	}
	if (this->IsStop == true) {
		if (StopTime <= 0) {
			IsStop = false;
			StopTime = 30;
			vy = nextV;
		}
		else {
			StopTime--;
		}
	}

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	int xxx = mario->Getx();
	int yyy = mario->Gety();
	if (xxx <= x && yyy <= y) {
		this->SetState(PLANT_STATE_TOPLEFT);
	}
	else if(xxx< x && yyy>y)
	{
		this->SetState(PLANT_STATE_BOTLEFT);
	}
	else if (xxx > x && yyy < y) {
		this->SetState(PLANT_STATE_TOPRIGHT);
	}
	else if (xxx > x && yyy > y) {
		this->SetState(PLANT_STATE_BOTRIGHT);
	}

	CGameObject::Update(dt, coObjects);
	//CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CPlant::Render() {
	int RenderId = PLANT_SPRITE_TOPLEFT;
	switch (state) {
	case PLANT_STATE_TOPLEFT: {
		RenderId = PLANT_SPRITE_TOPLEFT;
		break;
	}
	case PLANT_STATE_TOPRIGHT: {
		RenderId = PLANT_SPRITE_TOPRIGHT;
		break;
	}
	case PLANT_STATE_BOTLEFT: {
		RenderId = PLANT_SPRITE_BOTLEFT;
		break;
	}
	case PLANT_STATE_BOTRIGHT: {
		RenderId = PLANT_SPRITE_BOTRIGHT;
		break;
	}
	default:
		break;
	}
	CSprites* s = CSprites::GetInstance();
	s->Get(RenderId)->Draw(x, y);
}
void CPlant::SetState(int state){
	CGameObject::SetState(state);
}