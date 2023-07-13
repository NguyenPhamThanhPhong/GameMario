#include "Huditem.h"
#include "Game.h"


void CHud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	float xx = 0, yy = 0;
	CGame::GetInstance()->GetCamPos(xx,yy);
	x = xx;
	y = yy+50;
}
void CHud::Render() {
	CSprites* s = CSprites::GetInstance();
	s->Get(spriteId)->Draw(x, y);
}