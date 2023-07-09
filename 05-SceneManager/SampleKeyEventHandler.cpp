#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"
#include "Tail.h"

#include "Mario.h"
#include "Turtle.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_FOX);
		break;
	case DIK_A:
		mario->SetHoldingkey(true);
		break;
	case DIK_X: {
		if (mario->Getlevel() == MARIO_LEVEL_FOX) {
			mario->SetTailTriggering(true);
			CTail* tail = (CTail*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetTail();
			tail->SetState(TAIL_TRIGGER);
		}
		break;
	}
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPPLAYSCENE currentScene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());
	switch (KeyCode)
	{
	case DIK_S:
		if (abs(mario->GetVx()) >= 0.2f) {
			mario->SetState(MARIO_STATE_FLY);
			break;
		}
		else {
			mario->SetState(MARIO_STATE_RELEASE_JUMP);

		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_X: {
		CTail* tail = (CTail*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetTail();
		tail->SetState(TAIL_SLEEP);
		mario->SetTailTriggering(false);
		break;
		}
	case DIK_A:
	{
		mario->SetHoldingkey(false);
		CTurtle* turtle = (CTurtle*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetTurtle();
		if (turtle != NULL) {
			if (turtle->GetState() == TURTLE_SLEEP_HOLD) {
				if (mario->GetnX() > 0)
					turtle->SetState(TURTLE_SPIN_RIGHT);
				else if (mario->GetnX() < 0)
					turtle->SetState(TURTLE_SPIN_LEFT);
			}
		}
		break;
	}

	case DIK_4:
		currentScene->UnlockBreakableBricks();
		break;
	}

}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}