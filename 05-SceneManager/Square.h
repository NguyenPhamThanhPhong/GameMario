#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Brick.h"

#define ANI_ID_SQUARE 300000

class CSquare : public CGameObject
{
protected:
	//int Width;
	//int Height;
	//float cellWidth;
	//float cellHeight;
	//int spriteIdEdgeLeft, spriteIdEdgeRight, spriteIdEdgeTop, spriteIdEdgeBot; // Edge
	//int spriteIdTopLeft, spriteIdTopRight, spriteIdBotLeft, spriteIdBotRight; // Corner
	//int spriteIdContent;

public:

	CSquare(float x, float y) :CGameObject(x, y) {

	}
	//renderrr
	void Render() {
		int cellWidth = 16;
		int cellHeight = 16;
		CSprites* s = CSprites::GetInstance();
		s->Get(20001)->Draw(x, y);
		s->Get(20001)->Draw(x + cellWidth, y);
		s->Get(20001)->Draw(x + cellWidth + cellWidth, y);
		s->Get(20001)->Draw(x + cellWidth + cellWidth, y + cellHeight);
		s->Get(20001)->Draw(x + cellWidth + cellWidth, y + cellHeight + cellHeight);
		s->Get(20001)->Draw(x + cellWidth, y + cellHeight + cellHeight);
		s->Get(20001)->Draw(x, y + cellHeight + cellHeight);
		s->Get(20001)->Draw(x, y + cellHeight);
		s->Get(20001)->Draw(x + cellWidth, y + cellHeight);
	}
	//end rendderrr

	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = this->x - 16 / 2;
		t = this->y - 16 / 2;
		r = l + 48;
		b = r + 48;
	}
	int IsDirectionCollidable(float nx, float ny) {
		if (ny < 0) {
			return 1;
		}
		return 0;
	}
};

//CSquare(float x, float y,
//	float cell_width, float cell_height, int width, int height,
//	int sprite_id_edge_left, int sprite_id_edge_right, int sprite_id_edge_top, int sprite_id_edge_bot,
//	int sprite_id_content,
//	int sprite_id_top_left, int sprite_id_top_right,
//	int sprite_id_bot_left, int sprite_id_bot_right) : CGameObject(x, y)
//{
//	this->Width = width;
//	this->Height = height;
//	this->cellWidth = cell_width;
//	this->cellHeight = cell_height;
//	//Edge
//	this->spriteIdEdgeLeft = sprite_id_edge_left;
//	this->spriteIdEdgeRight = sprite_id_edge_right;
//	this->spriteIdEdgeTop = sprite_id_edge_top;
//	this->spriteIdEdgeBot = sprite_id_edge_bot;
//	//Content
//	this->spriteIdContent = sprite_id_content;
//	//Corner
//	this->spriteIdTopLeft = sprite_id_top_left;
//	this->spriteIdTopRight = sprite_id_top_right;
//	this->spriteIdBotLeft = sprite_id_bot_left;
//	this->spriteIdBotRight = sprite_id_bot_right;
//}