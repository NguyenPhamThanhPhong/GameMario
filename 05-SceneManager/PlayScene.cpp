#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Pipe.h"
#include "Square.h"
#include "Plant.h"
#include "CoinBounce.h"
#include "Mushroom.h"
#include "testobj.h"
#include "BackgroundObj.h"
#include "FireBall.h"
#include "Turtle.h"
#include "InvisibleBlock.h"
#include "Leaf.h"
#include "Flygoomba.h"
#include "Tail.h"
#include "BreakableBrick.h"
#include "Button.h"
#include "MarioIcon.h"
#include "Node.h"
#include "BackgroundAni.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	if (id == 3) {
		key_handler = new CSampleKeyHandlerWorld(this);
	}
	else
		key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		//DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;
		//int level = CGame::GetInstance()->getPlayerlevel();
		//((CMario*)player)->SetLevel(level);

		//DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_MYSTERIC_BRICK: {
		int rewardid = (int)atof(tokens[3].c_str());
		obj = new CMystericBrick(x, y,rewardid);
		break;

	}
	case OBJECT_TYPE_COIN_BOUNCE: {
		 int rewardindex = (int)atof(tokens[3].c_str());
		obj = new CCoinBounce(x, y,rewardindex); 
		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		int spriteId = (int)atof(tokens[5].c_str());
		obj = new CPipe(x, y, width, height, spriteId);
		break;
	}
	case OBJECT_TYPE_PLANT:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		float topmost = (float)atof(tokens[5].c_str());
		float botmost = (float)atof(tokens[6].c_str());
		int index = (int)atoi(tokens[7].c_str());
		obj = new CPlant(x, y, width, height, topmost, botmost,index);
		break;
	}
	case 11:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		int spriteId = atoi(tokens[5].c_str());
		int rewardindex = (int)atof(tokens[6].c_str());
		obj = new CMushroom(x, y, width, height, spriteId,rewardindex);
		break;
	}
	case 12: {
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		float width = (float)atof(tokens[5].c_str());
		float height = (float)atof(tokens[6].c_str());

		int idEdgeLeft = (int)atof(tokens[7].c_str());
		int idEdgeRight = (int)atof(tokens[8].c_str());
		int idEdgeTop = (int)atof(tokens[9].c_str());
		int idEdgeBot = (int)atof(tokens[10].c_str());

		int idContent = (int)atof(tokens[11].c_str());

		int idTopLeft = (int)atof(tokens[12].c_str());
		int idTopRight = (int)atof(tokens[13].c_str());
		int idBotLeft = (int)atof(tokens[14].c_str());
		int idBotRight = (int)atof(tokens[15].c_str());
		obj = new Ctest(
			x, y, cell_width, cell_height,
			width, height,
			idEdgeLeft, idEdgeRight, idEdgeTop, idEdgeBot,
			idContent,
			idTopLeft, idTopRight, idBotLeft, idBotRight
		);
		break;
	}
	case 15: {
		int spriteId = (int)atoi(tokens[3].c_str());
		obj = new CBackgroundobj(x, y, spriteId);
		break;
	}
	case 16: {
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		int spriteId = atoi(tokens[5].c_str());
		obj = new CFireball(x, y, width, height, spriteId);
		fireball = (CFireball*)obj;
		DebugOut(L"[INFO] Fire ball created\n");
		break;
	}
	case 17: {
		int colorstate1 = (int)atoi(tokens[3].c_str());
		int initialstate = (int)atoi(tokens[4].c_str());
		if (colorstate1 != 1 && colorstate1 != 2)
			colorstate1 = 2;
		obj = new CTurtle(x, y,colorstate1,initialstate);
		break;
	}
	case 18:
	{
		obj = new CTransparentblock(x, y);
		break;
	}
	case 19:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		int rewardindex = (int)atof(tokens[5].c_str());
		obj = new CLeaf(x,y,width,height,rewardindex);
		break;
	}
	case 20:
	{
		float leftmost = (float)atof(tokens[3].c_str());
		float rightmost = (float)atof(tokens[4].c_str());
		obj = new CFlygoomba(x, y,leftmost,rightmost);
		break;
	}
	case 21:
	{
		obj = new CTail(x, y);
		tail = (CTail*)obj;
		break;
	}
	case 22:
	{
		float coinid = (float)atof(tokens[3].c_str());
		obj = new CBreakableBrick(x, y,coinid);
		break;
	}
	case 23:
	{
		float coinid = (float)atof(tokens[3].c_str());
		obj = new CCoinHidden(x, y, coinid);
		hidddenCoins.push_back(obj);
		break;
	}
	case 24:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		int spriteId = atoi(tokens[5].c_str());
		obj = new CFireball(x, y, width, height, spriteId);
		fireball2 = (CFireball*)obj;
		DebugOut(L"[INFO] Fire ball 2 (seccond) created\n");
		break;
	}
	case 25: {
		int rewardindex = (int)atof(tokens[3].c_str());
		obj = new CButton(x, y, rewardindex);
		break;
	}
	case 26: {
		obj = new CMarioIcon(x, y);
		marioicon = (CMarioIcon*)obj;
		break;
	}
	case 27: {
		int Left = (int)atof(tokens[3].c_str());
		int Right = (int)atof(tokens[4].c_str());
		int Top = (int)atof(tokens[5].c_str());
		int Bot = (int)atof(tokens[6].c_str());
		int isStage = (int)atof(tokens[4].c_str());
		obj = new CNode(x, y, Left, Right, Top, Bot, isStage);
		break;
	}
	case 28: {
		int aniId = (int)atof(tokens[3].c_str());
		obj = new CBackgroundAni(x, y, aniId);
		break;
	}
	case 29: {
		obj = new CCard(x, y);
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	//DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	//DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();


	if (id == 5) {
		DebugOut(L"[INFO] scene %d time: %d \n",id, time);
		if (time <= 0 || game_start <=0) {
			game_start = GetTickCount64();
			((CMario*)(player))->SetLevel(player_level);
		}
	}
	//DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	if (id == 5 || id == 1) {
		time = GAME_TIME-	(int)((GetTickCount64() - game_start)/1000);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	if (isWin == true && GetTickCount64() - win_start > 2000) {
		CGame::GetInstance()->InitiateSwitchScene(3, 400, 300);
		DebugOut(L"[INFO] game win \n");
		isWin = false;
		return;
	}
	if (!isWin && id==5||id ==1) {
		if (time <= 0)
			SetGameOver();
	}

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < 0) cx = 0;
	if (cx > 2700) cx = 2700;

	if (cy > 228) cy = 225;
	else if (cy < 10) cy = 0;

	if (id == 3 || id == 4) {
		cx = 0;
		cy = 0;
	}
	CGame::GetInstance()->SetCamPos(cx, cy /*cy*/);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	if (id == 5 || id == 1 || id == 3) {
		float xx = 0, yy = 0;
		CGame::GetInstance()->GetCamPos(xx,yy);
		CSprites* s = CSprites::GetInstance();
		xx += 160;
		yy += 190;
		s->Get(111)->Draw(xx, yy);
		s->Get(800032)->Draw(xx, yy);


		float scorex = xx - 14;
		float scorey = yy + 4;
		Rendernums(7, score, scorex, scorey);

		float timex = xx + 28;
		float timey = scorey;
		Rendernums(3, time, timex, timey);
		

		float coinx = timex+10;
		float coiny = timey - 10;
		Rendernums(2, coin, coinx, coiny);

		if (isWin)
		{
			s->Get(199012)->Draw(2690,265);
			if (card <= 3 && card>=1) {
				s->Get(199018+card)->Draw(2720, 265);

			}
		}

	}

	if (id == 3 && WasgameOver == true) {
		CSprites* s = CSprites::GetInstance();
		s->Get(199023)->Draw(150, 50);
	}
}
void CPlayScene::Rendernums(int size, int num, float numx, float numy) {
	//render score
	int digits[7] = { 0 };  // Initialized with all zeros

	// Extract digits in reverse order
	int numDigits = 0;
	while (num > 0 && numDigits < 7) {
		digits[numDigits] = num % 10;
		num /= 10;
		numDigits++;
	}
	CSprites* s = CSprites::GetInstance();
	for (int i = 0; i < size; i++) {
		s->Get(101 + digits[i])->Draw(numx, numy);
		numx -= 8;
	}
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::SetGameOver() {
	CGame::GetInstance()->InitiateSwitchScene(3, 400, 300);
	CGame::GetInstance()->SetGameWasOver();
	score = 0;
	coin = 0;
}
void CPlayScene::SetGameWin(int received_card) {
	isWin = true;
	win_start = GetTickCount64();
	card = received_card;
	CGame::GetInstance()->SetScoreTimeCoinGlobal(score, 0, coin);
}
void CPlayScene::SetScoreTimeCoin(int scored, ULONGLONG game_startz, int coined) {
	this->coin = coined;
	this->score = scored;
	this->game_start = game_startz;

	int timetemp = GAME_TIME - (int)((GetTickCount64() - game_start) / 1000);
	DebugOut(L" scene id(%d) time in scene: %d \n ",id, timetemp);

}