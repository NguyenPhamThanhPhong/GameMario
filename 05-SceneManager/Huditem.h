#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CHud : public CGameObject {
private:
	int spriteId=800032;
	int coin=0;
	int score = 0;
	int percent = 0;
public:
	CHud(float x, float y) : CGameObject(x, y) {
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	int IsBlocking() { return 0; }
};