#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define BUTTON_SLEEP 1
#define BUTTON_TRIGGER 2
#define BUTTON_DIE 3

class CButton : public CGameObject
{
private:
	int Rewardindex;
protected:
	virtual int IsCollidable() { return 1; };

public:
	CButton(float x, float y, int rewardid) : CGameObject(x, y) {
		this->Rewardindex = rewardid;
		state = 1;
	}
	virtual int GetRewardIndex() { return this->Rewardindex; }

	virtual void SetState(int state);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};