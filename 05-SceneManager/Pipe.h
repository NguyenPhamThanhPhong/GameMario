#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CPipe : public CGameObject {
private:
	float width;
	float height;
	int spriteID;
public:
	CPipe(float x, float y,float width,float height,int spriteID) : CGameObject(x, y) {
		this->width = width;
		this->height = height;
		this->spriteID = spriteID;
	}
	void Render()
	{
		CSprites* s = CSprites::GetInstance();

		s->Get(this->spriteID)->Draw(x, y);

		//CAnimations* animations = CAnimations::GetInstance();
		//animations->Get(10000)->Render(x, y);
		//RenderBoundingBox();
	}
	void Update(DWORD dt) {}
	virtual int IsBlocking() { return 1; }
	void GetBoundingBox(float& l, float& t, float& r, float& b)
	{
		l = x - width / 2;
		t = y - height / 2;
		r = l + width;
		b = t + height;
	}
};