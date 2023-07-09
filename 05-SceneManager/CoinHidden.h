#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define COIN_HIDDEN_DIE 1
#define COIN_HIDDEN_LIVE 2
#define COIN_HIDDEN_SLEEP 3


class CCoinHidden : public CGameObject {
private:
	int id;
protected:
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
public:
	CCoinHidden(float x, float y,int id) : CGameObject(x, y) {
		state = COIN_HIDDEN_SLEEP;
		this->id = id;
	}
	int Getid() { return this->id; }
	void Render() {
		if (state == COIN_HIDDEN_LIVE) {
			CSprites* s = CSprites::GetInstance();
			s->Get(40001)->Draw(x, y);
		}
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		DebugOut(L"[ERROR] Texture ID %d \n", state);

		CGameObject::Update(dt, coObjects);
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {

		float width = 15;
		float height = 15;
		left = x - width / 2;
		top = y - height / 2;
		right = x + width;
		bottom = y + height;
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
		switch (state) {
		case COIN_HIDDEN_DIE:
		{
			isDeleted = true;
			break;
		}
		}
	}

};