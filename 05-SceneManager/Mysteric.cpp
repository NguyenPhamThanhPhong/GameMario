#pragma once
#include "Brick.h"

#define ID_ANI_MYSTERIC_BRICK 100000
#define ID_ANI_MYSTERIC_BRICK_DIE 100001

#define MYSTERIC_BRICK_HEIGHT 17
#define MYSTERIC_BRICK_WIDTH 16


void CMystericBrick::Render()
{
	int renderID = ID_ANI_MYSTERIC_BRICK;
	if (state == MYSTERIC_STATE_DIE)
		renderID = ID_ANI_MYSTERIC_BRICK_DIE;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(renderID)->Render(x, y);
	RenderBoundingBox();
}
void CMystericBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MYSTERIC_BRICK_WIDTH / 2;
	t = y - MYSTERIC_BRICK_HEIGHT / 2;
	r = l + MYSTERIC_BRICK_WIDTH;
	b = t + MYSTERIC_BRICK_HEIGHT;
}
void CMystericBrick::SetState(int state)
{
	CGameObject::SetState(state);
}