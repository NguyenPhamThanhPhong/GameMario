#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CBackgroundAni : public CGameObject {
private:
	int aniId;
public:
	CBackgroundAni(float x, float y, int aniId) : CGameObject(x, y) {
		this->aniId = aniId;
	}
	void Render() {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(aniId)->Render(x, y);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	int IsBlocking() { return 0; }
};