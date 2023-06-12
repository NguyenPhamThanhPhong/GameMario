#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16


#define MYSTERIC_STATE_LIVE 100
#define MYSTERIC_STATE_DIE 200


class CBrick : public CGameObject {
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
class CMystericBrick : public CBrick
{
protected:
	virtual int IsCollidable() { return 1; };

public:
	CMystericBrick(float x, float y) : CBrick(x, y) {}
	virtual void SetState(int state);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
