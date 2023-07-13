#include "Tail.h"
#include "BreakableBrick.h"
#include "Brick.h"
#include "debug.h"

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float xxx = mario->Getx();
	float yyy = mario->Gety();
	y = yyy + 5;

	if (x <= xxx - 10) {
		x = xxx - 9;
		vx = 0.4f;
	}
	else if (x >= xxx + 10) {
		x = xxx + 9;
		vx = -0.4f;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::OnNoCollision(DWORD dt) {
	x += vx;
	//x = xxx;
	//y = yyy;
}
void CTail::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (state == TAIL_SLEEP) {
		return;
	}
	else {
		if (dynamic_cast<CBreakableBrick*>(e->obj)) {
			CBreakableBrick* bbrick = dynamic_cast<CBreakableBrick*>(e->obj);
			if (bbrick->GetState() == 1) {
				bbrick->SetState(3);
			}
		}
		else if (dynamic_cast<CMystericBrick*>(e->obj)) {
			CMystericBrick* b = dynamic_cast<CMystericBrick*>(e->obj);
			DebugOut(L"mysteric reward: %d \n",b->GetClaimid());

			if (b->GetClaimid() == 101) {
				b->SetState(MYSTERIC_STATE_DIE);
			}
		}
	}
}