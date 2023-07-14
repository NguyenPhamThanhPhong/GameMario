#pragma once

#include "Scene.h"

class CSampleKeyHandler: public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CSampleKeyHandler(LPSCENE s) :CSceneKeyHandler(s) {};
};

class CSampleKeyHandlerWorld : public CSceneKeyHandler {
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CSampleKeyHandlerWorld(LPSCENE s) :CSceneKeyHandler(s) {};
};
class CSampleKeyHandlerTitle : public CSceneKeyHandler {
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CSampleKeyHandlerTitle(LPSCENE s) :CSceneKeyHandler(s) {};
};
