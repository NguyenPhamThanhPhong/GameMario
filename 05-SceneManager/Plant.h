#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CPlant : CGameObject
{
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	int IsEmerged;
public:
	CPlant(int x, int y) : CGameObject(x, y){}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
