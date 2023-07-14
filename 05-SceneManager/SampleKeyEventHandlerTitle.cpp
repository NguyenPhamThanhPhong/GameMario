#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"


#include "PlayScene.h"

#define PLAYER1_X 100
#define PLAYER1_Y 137

#define PLAYER2_X 100
#define PLAYER2_Y 152

void CSampleKeyHandlerTitle::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	LPPLAYSCENE scene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());
	LPGAMEOBJECT arrow = scene->GetArrow();
	bool isAniPlaying = scene->GetisAniPlaying();
	switch (KeyCode)
	{
	case DIK_Q:
		if (isAniPlaying) {
			arrow->SetPosition(500,500);
			break;
		}
		else
		{
			float arrowx = 0, arrowy = 0;
			arrow->GetPosition(arrowx, arrowy);
			if (arrowx == PLAYER1_X && arrowy == PLAYER1_Y) {
				arrow->SetPosition(PLAYER2_X, PLAYER2_Y);
			}
			else
				arrow->SetPosition(PLAYER1_X, PLAYER1_Y);
		}
		break;
	case DIK_W:
		if (isAniPlaying)
			scene->SetAniPlaying(false);
		else
			CGame::GetInstance()->InitiateSwitchScene(3, 400, 400, 0, 0);
		break;
	}
}

void CSampleKeyHandlerTitle::OnKeyUp(int KeyCode)
{

}

void CSampleKeyHandlerTitle::KeyState(BYTE* states)
{
}