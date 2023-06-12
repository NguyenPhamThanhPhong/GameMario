#pragma once
//#include "Square.h"
//
//void CSquare::Render()
//{
//	//if (Width < 3* cellWidth || Height < 3* cellHeight) return;
//	//int sizeHor = Width / cellWidth; // numbers of cells horizontally
//	//int sizeVer = Height / cellHeight;// number of cells vertically
//	////Declare reuseable start point
//	//float xLocation = x;
//	//float yLocation = y;
//	//CSprites* s = CSprites::GetInstance();
//	//// Draw Top Left
//	//s->Get(spriteIdTopLeft)->Draw(x, y); 
//	////Draw Edge Top
//	//xLocation += this->cellWidth;
//	//for (int i = 0; i < sizeHor-2; i++) {
//	//	s->Get(spriteIdEdgeTop)->Draw(xLocation, yLocation);
//	//	xLocation += this->cellWidth;
//	//}
//	////Draw TopRight
//	//s->Get(spriteIdTopRight)->Draw(xLocation, yLocation);
//	////Draw Edge Right
//	//yLocation += this->cellHeight;
//	//for (int i = 0; i < sizeVer - 2; i++) {
//	//	s->Get(spriteIdEdgeRight)->Draw(xLocation, yLocation);
//	//	yLocation += this->cellHeight;
//	//}
//	////Draw BotRight
//	//s->Get(spriteIdBotRight)->Draw(xLocation, yLocation);
//	////Draw Edge Bot
//	//xLocation -= this->cellWidth;
//	//for (int i = 0; i < sizeHor - 2; i++) {
//	//	s->Get(spriteIdEdgeBot)->Draw(xLocation, yLocation);
//	//	xLocation -= this->cellWidth;
//	//}
//	////Draw BotLeft
//	//s->Get(spriteIdBotLeft)->Draw(xLocation, yLocation);
//	////Draw Edge Left
//	//yLocation -= this->cellHeight;
//	//for (int i = 0; i < sizeVer - 2; i++) {
//	//	s->Get(spriteIdEdgeLeft)->Draw(xLocation, yLocation);
//	//	yLocation -= this->cellHeight;
//	//}
//	////Draw Content
//	//int xx = x + cellWidth;
//	//int yy = y + cellHeight;
//	//for (int i = 0; i < sizeHor; i++) {
//	//	for (int j = 0; j < sizeVer; j++) {
//	//		s->Get(spriteIdContent)->Draw(xLocation, yLocation);
//	//		yy += cellHeight;
//	//	}
//	//	xx += cellWidth;
//	//}
//	testrender();
//}
//void CSquare::testrender()
//{
//
//	CSprites* s = CSprites::GetInstance();
//	s->Get(spriteIdTopLeft)->Draw(x, y);
//	s->Get(spriteIdEdgeTop)->Draw(x+cellWidth, y);
//	s->Get(spriteIdTopRight)->Draw(x + cellWidth + cellWidth, y);
//	s->Get(spriteIdEdgeRight)->Draw(x + cellWidth + cellWidth, y + cellHeight);
//	s->Get(spriteIdBotRight)->Draw(x + cellWidth + cellWidth, y + cellHeight+cellHeight);
//	s->Get(spriteIdEdgeBot)->Draw(x + cellWidth , y + cellHeight + cellHeight);
//	s->Get(spriteIdBotLeft)->Draw(x, y + cellHeight + cellHeight);
//	s->Get(spriteIdEdgeLeft)->Draw(x , y + cellHeight);
//	s->Get(spriteIdContent)->Draw(x + cellWidth , y+cellHeight);
//	RenderBoundingBox();
//}
//
////void CSquare::GetBoundingBox(float& left, float& top, float& right, float& bottom)
////{
//	//left = this->x - cellWidth / 2;
//	//top = this->y - cellHeight / 2;
//	//right = left + this->Width;
//	//bottom = right + this->Height;
////}
//void CSquare::GetBoundingBox(float& l, float& t, float& r, float& b) {
//	l = this->x - cellWidth / 2;
//	t = this->y - cellHeight / 2;
//	r = l + this->Width;
//	b = r + this->Height;
//}
