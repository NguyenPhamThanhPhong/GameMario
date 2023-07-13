#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "MarioIcon.h"

#include "PlayScene.h"


void CSampleKeyHandlerWorld::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMarioIcon* icon = (CMarioIcon*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetMarioIcon();

	switch (KeyCode)
	{
	case DIK_LEFT:
		icon->SetState(MARIO_ICON_MOVE_LEFT);
		break;
	case DIK_RIGHT:
		icon->SetState(MARIO_ICON_MOVE_RIGHT);
		break;
	case DIK_DOWN:
		icon->SetState(MARIO_ICON_MOVE_BOT);
		break;
	case DIK_UP:
		icon->SetState(MARIO_ICON_MOVE_TOP);
		break;
	}
}

void CSampleKeyHandlerWorld::OnKeyUp(int KeyCode)
{

}

void CSampleKeyHandlerWorld::KeyState(BYTE* states)
{
}