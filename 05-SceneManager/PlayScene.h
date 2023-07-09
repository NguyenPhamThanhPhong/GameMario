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
	LPGAMEOBJECT fireball;
	LPGAMEOBJECT tail;
	LPGAMEOBJECT turtle = NULL;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> bbricks;
	vector<LPGAMEOBJECT> hidddenCoins;


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
	LPGAMEOBJECT GetFireball() { return fireball; }
	LPGAMEOBJECT GetTail() { return tail; }
	LPGAMEOBJECT GetTurtle() { return turtle; }
	LPGAMEOBJECT GetsingleCoinHidden(int index) { 
		if(index<hidddenCoins.size())
			return hidddenCoins[index]; 
		return NULL;
	}

	void UnlockBreakableBricks() {
		vector<LPGAMEOBJECT>::iterator it;
		//for (it = bbricks.begin(); it != bbricks.end(); it++)
		//{
		//	if (*it != NULL) {
		//		DebugOut(L"[INFO] COIN HIDDEN %d unloaded! \n", (*it)->GetState());

		//	}
		//}
	};
	void Clear();
	void PurgeDeletedObjects();
	void SetTurtle(LPGAMEOBJECT currentTurtle) { this->turtle = currentTurtle; }

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

