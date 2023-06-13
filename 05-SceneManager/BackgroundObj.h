#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CBackgroundobj : public CGameObject {
private:
	int spriteId;
public:
	CBackgroundobj(float x, float y,int spriteId) : CGameObject(x, y) {
		this->spriteId = spriteId;
	}
	void Render() {
		CSprites* s = CSprites::GetInstance();
		s->Get(spriteId)->Draw(x, y);

	}
	void GetBoundingBox(float& l, float& t, float& r, float& b){}
	int IsBlocking() { return 0; }
};