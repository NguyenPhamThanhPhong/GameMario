#pragma once
#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CTransparentblock : public CGameObject {
public:
	CTransparentblock(float x, float y) : CGameObject(x, y) {
	}
	void Render() {
		CSprites* s = CSprites::GetInstance();
		s->Get(20001)->Draw(x, y);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - 10 / 2;
		t = y - 10 / 2;
		r = x + 10;
		b = y + 10;
	}
	int IsBlocking() { return 0; }
};