#include "Button.h"
#include "PlayScene.h"

void CButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float width = 10.0;
	float height = 10.0;
	l = x - width / 2;
	t = y - height / 2;
	r = l + width;
	b = t + height;
}
void CButton::SetState(int state) {
	CGameObject::SetState(state);
	LPPLAYSCENE currentScene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());

	if (state == 3) {
		currentScene->UnlockBreakableBricks();
	}
}
void CButton::Render() {
	if (state == 2) {
		int renderid = 810001;
		CSprites* s = CSprites::GetInstance();
		s->Get(renderid)->Draw(x, y);
	}
	//else if (state == 3) {
	//	int renderid = 900001;
	//	CSprites* s = CSprites::GetInstance();
	//	s->Get(renderid)->Draw(x+10, y+10);
	//}

}