#pragma once

#include "debug.h"

#include "MarioIcon.h"
#include "Node.h"

void CMarioIcon::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	float width = 10.0f;
	float height = 10.0f;
	left = x - width / 2;
	top = y - height / 2;
	right = x + width;
	bottom = y + height;
}
void CMarioIcon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CMarioIcon::Render() {
	CSprites* s = CSprites::GetInstance();

	s->Get(20001)->Draw(x,y);
	RenderBoundingBox();

}
void CMarioIcon::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}
void CMarioIcon::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CNode*>(e->obj)) {
		CNode* node = dynamic_cast<CNode*>(e->obj);
		node->GetDirections(Left,Right, Top, Bot);
		isAtStage = node->GetisStage();
		x = node->Getx();
		y = node->Gety();
		DebugOut(L"[INFO] now at : %d \n",(int)x);
		vx = 0;
		vy = 0;
	}
}

void CMarioIcon::SetState(int state) {
	if (vx != 0 || vy != 0) {
		return;
	}
	switch (state)
	{
	case MARIO_ICON_MOVE_LEFT:
		if (Left)
			vx = -ICON_VLOC;

		DebugOut(L"[INFO] left is : %d \n", Left);

		break;
	case MARIO_ICON_MOVE_RIGHT:
		if (Right)
			vx = ICON_VLOC;

		DebugOut(L"[INFO] right is : %d \n", Right);
		break;
	case MARIO_ICON_MOVE_TOP:
		if (Top)
			vy = -ICON_VLOC;
		DebugOut(L"[INFO] top is : %d \n", Top);
		break;
	case MARIO_ICON_MOVE_BOT:
		if (Bot)
			vy = ICON_VLOC;
		break;
	default:
		return;
	}
	CGameObject::SetState(state);


}