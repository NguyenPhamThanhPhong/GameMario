#pragma once
#include "Brick.h"
#include "PlayScene.h"

#define ID_ANI_MYSTERIC_BRICK 93
#define ID_ANI_MYSTERIC_BRICK_DIE 94

#define MYSTERIC_BRICK_HEIGHT 17
#define MYSTERIC_BRICK_WIDTH 16


void CMystericBrick::Render()
{
	int renderID = ID_ANI_MYSTERIC_BRICK;
	if (state == MYSTERIC_STATE_DIE)
		renderID = ID_ANI_MYSTERIC_BRICK_DIE;
	CAnimations* animations = CAnimations::GetInstance();

	if (rewardid == 101) {
		animations->Get(92)->Render(x, y);
		return;
	}

	animations->Get(renderID)->Render(x, y);
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
	if (state == MYSTERIC_STATE_DIE) {
		LPPLAYSCENE currentScene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());
		int claimID = Trigger_Reward_Bylevel();
		DebugOut(L">>> reward id: %d >>> \n",claimID);
		currentScene->TriggerRewards(claimID);
	}
}
int CMystericBrick::Trigger_Reward_Bylevel() {
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int level = mario->Getlevel();
	if (rewardid > 50)
		return rewardid;
	else if (rewardid == -1)
		return -2;
	switch (level)
	{
	case 1:
		if ((rewardid < 10 && rewardid>0))
			return rewardid;
		else
			return rewardid % 10;
	case 2:
		if ((rewardid < 20 && rewardid>10))
			return rewardid;
		else
			return rewardid % 10 + 10;
	case 3:
		if ((rewardid < 20 && rewardid>10))
			return rewardid;
		else
			return rewardid % 10;
	default:
		return -2;
	}
}