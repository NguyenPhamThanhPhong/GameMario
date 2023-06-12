#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Brick.h"

#define ANI_ID_SQUARE 300000

class CSquare : public CGameObject
{
protected:
	int Width;
	int Height;
	float cellWidth;
	float cellHeight;
	int spriteIdEdgeLeft, spriteIdEdgeRight, spriteIdEdgeTop, spriteIdEdgeBot; // Edge
	int spriteIdTopLeft, spriteIdTopRight, spriteIdBotLeft, spriteIdBotRight; // Corner
	int spriteIdContent;

public:
	CSquare(float x, float y,
		float cell_width, float cell_height, int width,int height,
		int sprite_id_edge_left, int sprite_id_edge_right, int sprite_id_edge_top,int sprite_id_edge_bot, 
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
		state = 0;

	}
	int IsCollidable() { return 0; };
	int IsDirectionCollidable(float nx, float ny) {
		if (nx == 0 && ny == -1) {
			return true;
		}
		return false;
	};
	int IsBlocking() { return 1; }
	void Render();
	void testrender();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};