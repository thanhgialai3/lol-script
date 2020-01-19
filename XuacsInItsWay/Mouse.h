#pragma once
#include <Windows.h>

#ifndef MOUSE_H
#define MOUSE_H

class Mouse {
public:
	const static DWORD timeToClick = 0;

	static void Move(int x, int y);
	static void MoveTo(int x, int y);

	static void KeyboardClick(int vKey);
	static void KeyboardClickDown(int vKey);
	static void KeyboardClickUp(int vKey);

	static void RightClick();

	static void LeftClick();

	static void ScriptClick(int, int);
	static bool IsKeyPressed(int);

};

#endif

