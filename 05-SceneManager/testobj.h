#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define SHADOWBOX_TOPRIGHT 200056
#define SHADOWBOX_BOTRIGHT 200057
#define SHADOWBOX_BOTLEFT 200058
#define SHADOWBOX_CONTENT 200059

#define MYSTERIC_STATE_LIVE 100
#define MYSTERIC_STATE_DIE 200


class Ctest : public CGameObject {
private:
	int Width;
	int Height;
	float cellWidth;
	float cellHeight;
	int spriteIdEdgeLeft, spriteIdEdgeRight, spriteIdEdgeTop, spriteIdEdgeBot; // Edge
	int spriteIdTopLeft, spriteIdTopRight, spriteIdBotLeft, spriteIdBotRight; // Corner
	int spriteIdContent;
public:
	Ctest(float x, float y,
	float cell_width, float cell_height, float width, float height,
	int sprite_id_edge_left, int sprite_id_edge_right, int sprite_id_edge_top, int sprite_id_edge_bot,
	int sprite_id_content,
	int sprite_id_top_left, int sprite_id_top_right,
	int sprite_id_bot_left, int sprite_id_bot_right) : CGameObject(x, y)
{
	this->Width = width;
	this->Height = height;
	this->cellWidth = cell_width;
	this->cellHeight = cell_height;
	//Edge
	this->spriteIdEdgeLeft = sprite_id_edge_left;
	this->spriteIdEdgeRight = sprite_id_edge_right;
	this->spriteIdEdgeTop = sprite_id_edge_top;
	this->spriteIdEdgeBot = sprite_id_edge_bot;
	//Content
	this->spriteIdContent = sprite_id_content;
	//Corner
	this->spriteIdTopLeft = sprite_id_top_left;
	this->spriteIdTopRight = sprite_id_top_right;
	this->spriteIdBotLeft = sprite_id_bot_left;
	this->spriteIdBotRight = sprite_id_bot_right;
}
	void Render() {

		if (Width < 2* cellWidth || Height < 2* cellHeight) return;

		CSprites* s = CSprites::GetInstance();
		int sizeHor = (int)(Width / cellWidth); // numbers of cells horizontally
		int sizeVer = (int)(Height / cellHeight);// number of cells vertically

		if (spriteIdTopLeft != 168011) {
			float xshadow = this->x + Width - 5;
			float yshadow = this->y + 4;
			int shadowboxHeightCount = (this->Height / 10) + 1;
			for (int i = 0; i < shadowboxHeightCount; i++)
			{
				s->Get(SHADOWBOX_CONTENT)->Draw(xshadow, yshadow);
				yshadow += 10;
			}
		}

		//Declare reuseable start point
		float xLocation = x;
		float yLocation = y;

		// Draw Top Left
		s->Get(spriteIdTopLeft)->Draw(x, y); 
		//Draw Edge Top
		xLocation += this->cellWidth;
		for (int i = 0; i < sizeHor-2; i++) {
			s->Get(spriteIdEdgeTop)->Draw(xLocation, yLocation);
			xLocation += this->cellWidth;
		}
		//Draw TopRight
		s->Get(spriteIdTopRight)->Draw(xLocation, yLocation);
		//Draw Edge Right
		yLocation += this->cellHeight;
		for (int i = 0; i < sizeVer - 2; i++) {
			s->Get(spriteIdEdgeRight)->Draw(xLocation, yLocation);
			yLocation += this->cellHeight;
		}
		//Draw BotRight
		s->Get(spriteIdBotRight)->Draw(xLocation, yLocation);
		//Draw Edge Bot
		xLocation -= this->cellWidth;
		for (int i = 0; i < sizeHor - 2; i++) {
			s->Get(spriteIdEdgeBot)->Draw(xLocation, yLocation);
			xLocation -= this->cellWidth;
		}
		//Draw BotLeft
		s->Get(spriteIdBotLeft)->Draw(xLocation, yLocation);
		//Draw Edge Left
		yLocation -= this->cellHeight;
		for (int i = 0; i < sizeVer - 2; i++) {
			s->Get(spriteIdEdgeLeft)->Draw(xLocation, yLocation);
			yLocation -= this->cellHeight;
		}
		//Draw Content
		float xx = x + cellWidth;
		for (int i = 0; i < sizeHor-2; i++) {
			float yy = y + cellHeight;
			for (int j = 0; j < sizeVer-2; j++) {
				s->Get(spriteIdContent)->Draw(xx, yy);
				yy += cellHeight;
			}
			xx += cellWidth;
		}
	}
	void DrawShadowBox() {

		//CSprites* s = CSprites::GetInstance();
		//yshadow += 8;
		//for (int i = 0; i < sizeVer; i++) {
		//	s->Get(SHADOWBOX_CONTENT)->Draw(xshadow, yshadow);
		//	y += 8;
		//}
		//s->Get(SHADOWBOX_BOTRIGHT)->Draw(xshadow, yshadow);
	}
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - cellWidth/ 2;
		t = y - cellHeight / 2;
		r = l + Width;
		b = t + Height;
	}
	int IsDirectionCollidable(float nx, float ny) { 
		if (ny < 0) {
			return 1;
		}
		return 0; }
};