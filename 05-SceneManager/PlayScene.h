#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;
	int player_level = 1;

	int score=986354;
	int time = 900;
	int coin;
	int card = -1;


	ULONGLONG win_start = -1;
	ULONGLONG game_start = -1;

	LPGAMEOBJECT fireball = NULL;
	LPGAMEOBJECT fireball2 = NULL;
	LPGAMEOBJECT tail;
	LPGAMEOBJECT turtle = NULL;

	LPGAMEOBJECT marioicon = NULL;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> hidddenCoins;

	bool isCoinDeleted = false;
	bool isWin = false;
	bool WasgameOver = false;


	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	LPGAMEOBJECT GetFireball(int index) { 
		if(index == 1)
			return fireball;
		return fireball2;
	}
	LPGAMEOBJECT GetTail() { return tail; }
	LPGAMEOBJECT GetTurtle() { return turtle; }

	void TriggerCoinHIdden(int index) {
		if (index < 0)	return;
		vector<LPGAMEOBJECT>::iterator it;
		for (it = objects.begin(); it != objects.end(); it++)
		{
			if ((*it)->GetidHiddenCoin() == index) {
				(*it)->SetState(2);
				return;
			}
		}
	}
	void DeleteCoinHIdden(int index) {
		if (index < 0)	return;
		vector<LPGAMEOBJECT>::iterator it;
		for (it = objects.begin(); it != objects.end(); it++)
		{
			if ((*it)->GetidHiddenCoin() == index) {
				(*it)->SetState(1);
				return;
			}
		}
	}
	LPGAMEOBJECT GetMarioIcon() { return marioicon; }
	void TriggerRewards(int rewardid) {
		if (rewardid < 0)
			return;
		vector<LPGAMEOBJECT>::iterator it;
		for (it = objects.begin(); it != objects.end(); it++)
		{
			if ((*it)->GetRewardIndex()== rewardid) {
				(*it)->SetState(2);
				return;
			}
		}
	}


	void UnlockBreakableBricks() {
		vector<LPGAMEOBJECT>::iterator it;
		for (it = objects.begin(); it != objects.end(); it++)
		{
			if ((*it)->IsControlledGroup() == true) {
				(*it)->SetState(2);

			}
		}
		isCoinDeleted = true;
	};
	void SetMarioStateGlobally(int lv) {
		player_level = lv;
	};

	void Rendernums(int size,int num,float numx,float numy);

	void SetGameOver();
	void SetGameWasOver(bool signal) { this->WasgameOver = signal; }
	void SetGameWin(int received_card);

	 
	bool GetisCoinDeleted() { return isCoinDeleted; }
	bool GetGameWasOver() { return this->WasgameOver; }

	void Clear();
	void PurgeDeletedObjects();
	void SetTurtle(LPGAMEOBJECT currentTurtle) { this->turtle = currentTurtle; }

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

