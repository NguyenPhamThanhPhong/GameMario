#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "Brick.h"
#include "Square.h"
#include "Plant.h"
#include "CoinBounce.h"
#include "Mushroom.h"
#include "FireBall.h"
#include "Turtle.h"
#include "Flygoomba.h"
#include "BreakableBrick.h"
#include "Button.h"
#include "BackgroundAni.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	//DebugOut(L">>> mario at: (%d,%d) >>> \n",(int)x,(int) y);
	if (isFlying) {
		if (GetTickCount64() - resetGravity_start > 300) {
			vy = 0.1f;
			ay = MARIO_GRAVITY;
			vx = 0.0f;
			isFlying = false;
			//SetState(MARIO_STATE_RELEASE_JUMP);
		}

	}
	else if (isGlide) {
		if (GetTickCount64() - glide_start > 300) {
			vy = 0.1;
			ay = MARIO_GRAVITY;
			vx = 0;
			isGlide = false;
		}
	}
	else if (wasAtMaxSpeed == false && level == MARIO_LEVEL_FOX && abs(vx)>=0.3f) {
		wasAtMaxSpeed = true;
		maxSpeed_start = GetTickCount64();
	}

	if ((wasAtMaxSpeed && abs(vx) < 0.3f) || level!= MARIO_LEVEL_FOX) {
		wasAtMaxSpeed = false;
	}

	if (this->state == MARIO_STATE_DETELEPORTDOWN || this->state == MARIO_STATE_DETELEPORTUP) {
		if (GetTickCount64() - deteleport_start > 1500) {
			ay = MARIO_GRAVITY;
			state = MARIO_STATE_IDLE;
			SetState(MARIO_STATE_IDLE);
			untouchable = 1;
			canCollide = true;
			if (isTeleporting) {
				ULONGLONG time = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Gettime();
				CGame::GetInstance()->SetScoreTimeCoinGlobal(score, time, coin);
				CGame::GetInstance()->InitiateSwitchScene(teleportto, startx, starty, score, coin);
			}
		}
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	if (x < 0)
		x = 0;
	y += vy * dt;
	if (y > 430)
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->SetGameOver();
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CMystericBrick*>(e->obj))
	{
		OnCOllisionWithMystericBrick(e);
	}
	else if (dynamic_cast<CPlant*>(e->obj)) {
		DamageMario();
	}
	else if (dynamic_cast<CMushroom*>(e->obj)) {
		CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
		if ( mushroom->GetState() == MUSHROOM_STATE_MOVE) {
			if (level == MARIO_LEVEL_SMALL) {
				SetLevel(MARIO_LEVEL_BIG);
			}
			e->obj->Delete();
		}
	}
	else if (dynamic_cast<CFireball*>(e->obj)) {
		DamageMario();
	}
	else if (dynamic_cast<CTurtle*>(e->obj)) {
		OnCollisionWithTurtle(e);
	}
	else if (dynamic_cast<CLeaf*>(e->obj)) {
		CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
		if (leaf->GetState() != LEAF_STATE_SLEEP)
		{
			if(level==MARIO_LEVEL_BIG)
				SetLevel(MARIO_LEVEL_FOX);
			e->obj->Delete();
			
		}
	}
	else if (dynamic_cast<CFlygoomba*>(e->obj)) {
		OnCollisionWithFlygoomba(e);
	}
	else if (dynamic_cast<CBreakableBrick*>(e->obj)) {
		CBreakableBrick* bbrick = dynamic_cast<CBreakableBrick*>(e->obj);
		if (e->ny > 0) {
			if (bbrick->GetState() == BREAKABLE_BRICK_LIVE) {
				bbrick->SetState(BREAKABLE_BRICK_DIE);
			}
		}
		if (bbrick->GetState() == BREAKABLE_BRICK_SLEEP) {
			bbrick->Killcoin();
			bbrick->Delete();
		}
	}
	else if (dynamic_cast<CButton*>(e->obj)) {
		CButton* button = dynamic_cast<CButton*>(e->obj);
		if (button->GetState() == 2) {
			button->SetState(3);
		}
	}
	else if (dynamic_cast<CCard*>(e->obj)) {
		DebugOut(L">>> get card %d >>> \n",e->obj->GetState());
		CCard* card = dynamic_cast<CCard*>(e->obj);
		if (card->GetState() == 1) {
			card->SetState(2);
		}
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			score += 100;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					if (level == MARIO_LEVEL_FOX)
						SetLevel(MARIO_LEVEL_BIG);
					else if (level == MARIO_LEVEL_BIG)
						SetLevel(MARIO_LEVEL_SMALL);
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithFlygoomba(LPCOLLISIONEVENT e) {
	CFlygoomba* flygoomba = dynamic_cast<CFlygoomba*>(e->obj);
	if (e->ny < 0) {
		if (flygoomba->GetState() == FLYGOOMBA_STATE_FLY||flygoomba->GetState()==FLYGOOMBA_STATE_FLYDOWN) {
			flygoomba->SetState(FLYGOOMBA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (flygoomba->GetState() == FLYGOOMBA_STATE_WALKING) {
			flygoomba->SetState(FLYGOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			score += 100;
		}
	}//hit by goomba
	else {
		if (untouchable == 0)
		{
			if (flygoomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					if (level == MARIO_LEVEL_FOX)
						SetLevel(MARIO_LEVEL_BIG);
					else if (level == MARIO_LEVEL_BIG)
						SetLevel(MARIO_LEVEL_SMALL);
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}

}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
	score += 50;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	LPGAME game = CGame::GetInstance();
	CPortal* p = (CPortal*)e->obj;
	this->startx = -10.0f;
	this->starty = -10.0f;
	if (e->ny < 0) {
		if (game->IsKeyDown(DIK_DOWN)) {
			teleportto = p->GetSceneId();
			if (p->GetSceneId() == 1) {
				startx = 150.0f;
				starty = 20.0f;
			}
			SetState(MARIO_STATE_DETELEPORTDOWN);
			isTeleporting = true;
		}
	}
	else if (e->ny > 0) {
		if (game->IsKeyDown(DIK_UP)) {
			teleportto = p->GetSceneId();
			if (p->GetSceneId() == 5) {
				startx = 2336.0f;
				starty = 345.0f;
			}
			SetState(MARIO_STATE_DETELEPORTUP);
			isTeleporting = true;
		}
	}



}
void CMario::OnCOllisionWithMystericBrick(LPCOLLISIONEVENT e)
{
	CMystericBrick* b = dynamic_cast<CMystericBrick*>(e->obj);
	if (e->ny > 0)
	{
		if (b->GetState() != MYSTERIC_STATE_DIE)
		{
			b->SetState(MYSTERIC_STATE_DIE);
			int claimid = b->GetClaimid();
			if (claimid != 101 && claimid > 50) {
				score += 50;
				coin++;
			}
			else if (claimid < 30 && claimid>0) {
				score += 100;
			}
		}
	}
}
void CMario::OnCollisionWithTurtle(LPCOLLISIONEVENT e) {
	CTurtle* turtle = dynamic_cast<CTurtle*>(e->obj);
	if (turtle->GetState() == TURTLE_FLY) {
		if (e->nx != 0 || e->ny > 0) {
			DamageMario();
		}
		else if (e->ny < 0) {
			turtle->SetState(TURTLE_LIVE);
		}
	}
	else if (turtle->GetState() == TURTLE_LIVE) {
		if (e->nx != 0 || e->ny > 0) {
			DamageMario();
		}
		else if (e->ny < 0) {
			turtle->SetState(TURTLE_SLEEP);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else if (turtle->GetState() == TURTLE_SLEEP) {
		if (IskeyHolding == true) {
			turtle->SetState(TURTLE_SLEEP_HOLD);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->SetTurtle(turtle);
		}
		else
		{
			if (e->ny != 0) {
				if (nx > 0)
					turtle->SetState(TURTLE_SPIN_RIGHT);
				else if (nx < 0)
					turtle->SetState(TURTLE_SPIN_LEFT);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else
			{
				if (e->nx < 0) {
					turtle->SetState(TURTLE_SPIN_RIGHT);
				}
				else if (e->nx > 0) {
					turtle->SetState(TURTLE_SPIN_LEFT);
				}
			}
		}
	}
	else if (turtle->GetState() == TURTLE_SPIN_LEFT || turtle->GetState() == TURTLE_SPIN_RIGHT) {
		if (e->ny < 0) {
			turtle->SetState(TURTLE_SLEEP);
			y -= 3;
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		DamageMario();
	}
	else if (turtle->GetState() == TURTLE_SLEEP) {
		if (nx > 0)
			turtle->SetState(TURTLE_SPIN_RIGHT);
		else if (nx < 0)
			turtle->SetState(TURTLE_SPIN_LEFT);
	}
}
void CMario::OnCollisionWidthSquare(LPCOLLISIONEVENT e)
{
	vy = 0;
	isOnPlatform = true;
	//CSquare* s = dynamic_cast<CSquare*>(e->obj);
	//if (e->ny < 0)
	//{
	//	vy = 0;
	//	if (e->ny < 0) isOnPlatform = true;
	//}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
	if (state == MARIO_STATE_DETELEPORTUP || state == MARIO_STATE_DETELEPORTDOWN) {
		aniId = ID_ANI_TELEPORT_SMALL;
	}

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;
	if (state == MARIO_STATE_DETELEPORTUP || state == MARIO_STATE_DETELEPORTDOWN) {
		aniId = ID_ANI_TELEPORT_BIG;
	}
	return aniId;
}
int CMario::GetAniIdFox()
{
	int aniId = 1700;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_FOX_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_FOX_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_FOX_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_FOX_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_FOX_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_FOX_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_FOX_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_FOX_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_FOX_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FOX_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_FOX_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_FOX_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FOX_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_FOX_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;
	if (isTriggeringTail == true) aniId = 1714;
	if (isFlying)
	{
		if(nx>0)
		aniId = ID_ANI_MARIO_FOX_FLY_RIGHT;
		else
			aniId = ID_ANI_MARIO_FOX_FLY_LEFT;

	}
	if (isGlide) {
		if (nx > 0)
			aniId = 1717;
		else
			aniId = 1718;

	}
	if (state == MARIO_STATE_DETELEPORTUP || state == MARIO_STATE_DETELEPORTDOWN) {
		aniId = ID_ANI_TELEPORT_FOX;
	}
	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_FOX)
		aniId = GetAniIdFox();
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

	DebugOutTitle(L"Coins: %d", coin);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->SetScoreCoin(score, coin);

}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;
	if (this->state == MARIO_STATE_DETELEPORTDOWN || this->state == MARIO_STATE_DETELEPORTUP)
		return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		if (isFlying) {
			vx = 0.2f;
			nx = 1;
			break;
		}
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		if (isFlying) {
			vx = -0.2f;
			nx = -1;
			break;
		}
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		if (isFlying) {
			vx = 0.2f;
			nx = 1;
			break;
		}
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		if (isFlying) {
			vx = -0.2f;
			nx = -1;
			break;
		}
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isFlying) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (level == MARIO_LEVEL_FOX) {
			if (vy > 0) {
				ay = 0;
				vy = 0.06f;
				isGlide = true;
				glide_start = GetTickCount64();
				break;
			}
		}
		else {
			if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		}
		break;
	case MARIO_STATE_FLY:
		ay = 0;
		vy = -0.2f;
		vx = 0.2f;
		isFlying = true;
		isOnPlatform = false;
		resetGravity_start = GetTickCount64();
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_DETELEPORTUP:
		vy = -0.01f;
		vx = 0.0f;
		ax = 0.0f;
		ay = 0.0f;
		untouchable = 0;
		canCollide = false;
		deteleport_start = GetTickCount64();
		break;
	case MARIO_STATE_DETELEPORTDOWN:
		vy = 0.01f;
		vx = 0.0f;
		ax = 0.0f;
		ay = 0.0f;
		untouchable = 0;
		canCollide = false;
		deteleport_start = GetTickCount64();
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FOX)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	CGame::GetInstance()->SetPlayerlevel(l);
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

