#include "Pipe.h"
#include "PlayScene.h"

int CPipe::IsBlocking() {
	LPPLAYSCENE currentScene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());
	if (currentScene->GetisWin()) {
		return 0;
	}
	else return 1;
}