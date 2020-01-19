#include "Mouse.h"

#include "Globals.h"

void Mouse::Move(int x, int y)
{
	DWORD nx = x * 65535 / GetSystemMetrics(0);
	DWORD ny = y * 65535 / GetSystemMetrics(1);

	//INPUT input;
	//input.type = INPUT_MOUSE;
	//input.mi.mouseData = 0;
	//input.mi.time = 0;
	//input.mi.dx = nx;//x being coord in pixels
	//input.mi.dy = ny;//y being coord in pixels
	//input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;

	mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE, nx, ny, 0, 0);

	POINT mouse_pos;
	GetCursorPos(&mouse_pos);

	if (mouse_pos.x != nx || mouse_pos.y != ny)
		mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE, nx, ny, 0, 0);

	//SendInput(1, &input, sizeof(input));
}


ULONGLONG waitingClick = 0;
void Mouse::RightClick()
{
	bool canMove = true;


	for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
	{
		luabridge::LuaRef ref = luabridge::getGlobal(LuaKeeper::lua_list[i], "CanMove");

		if (!ref.isNil() && ref.isBool())
		{

			if (ref == false)
			{
				canMove = false;
				break;
			}

		}

	}

	if (!canMove)
		return;

	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof(input));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	//if (GetTickCount64() >= waitingClick)
	//{
	//	INPUT input;
	//	input.type = INPUT_MOUSE;
	//	input.mi.mouseData = 0;
	//	input.mi.time = 0;
	//	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	//	SendInput(1, &input, sizeof(input));
	//	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	//	SendInput(1, &input, sizeof(input));

	//	waitingClick = GetTickCount64() + timeToClick;
	//}
}

void Mouse::LeftClick()
{
	//INPUT input;
	//input.type = INPUT_MOUSE;
	//input.mi.mouseData = 0;
	//input.mi.time = 0;
	//input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	//SendInput(1, &input, sizeof(input));

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	//input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	//SendInput(1, &input, sizeof(input));
}

#include <iostream>

void Mouse::ScriptClick(int x,int y)
{
	bool canMove = true;


	for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
	{
		luabridge::LuaRef ref = luabridge::getGlobal(LuaKeeper::lua_list[i], "CanMove");

		if (!ref.isNil() && ref.isBool())
		{

			if (ref == false)
			{
				canMove = false;
				break;
			}

		}

	}
	
	if (!canMove)
		return;


	Sleep(1);
	//BlockInput(TRUE);

	POINT oldMousePos;
	GetCursorPos(&oldMousePos);

	RECT rect = RECT();
	rect.bottom = y;
	rect.top = y;
	rect.right = x;
	rect.left = x;

	ClipCursor(&rect);

	//Mouse::Move(x, y);
	Sleep(10);
	//Mouse::LeftClick();
	Mouse::KeyboardClick('L');

	Sleep(10);
	ClipCursor(NULL);
	Sleep(1);
	Mouse::Move(oldMousePos.x, oldMousePos.y);
	//BlockInput(FALSE);
	Sleep(1);
}


void Mouse::MoveTo(int x, int y)
{
	bool canMove = true;


	for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
	{
		luabridge::LuaRef ref = luabridge::getGlobal(LuaKeeper::lua_list[i], "CanMove");

		if (!ref.isNil() && ref.isBool())
		{

			if (ref == false)
			{
				canMove = false;
				break;
			}

		}

	}

	if (!canMove)
		return;


	Sleep(1);

	POINT oldMousePos;
	GetCursorPos(&oldMousePos);

	RECT rect = RECT();
	rect.bottom = y;
	rect.top = y;
	rect.right = x;
	rect.left = x;

	ClipCursor(&rect);

	Sleep(10);
	Mouse::RightClick();

	Sleep(10);
	ClipCursor(NULL);
	Sleep(1);
	Mouse::Move(oldMousePos.x, oldMousePos.y);
	Sleep(1);
}

//void Mouse::ScriptClick(int x, int y)
//{
//	Mouse::KeyboardClick('A');
//
//	BlockInput(TRUE);
//	POINT oldMousePos;
//	GetCursorPos(&oldMousePos);
//	Sleep(10);
//	Mouse::Move(x, y);
//	Sleep(15);
//	Mouse::LeftClick();
//	Sleep(15);
//	Mouse::Move(oldMousePos.x, oldMousePos.y);
//	Sleep(5);
//	BlockInput(FALSE);
//}

bool Mouse::IsKeyPressed(int vKey)
{
	return GetAsyncKeyState(vKey);
}

void Mouse::KeyboardClick(int vKey)
{
	//INPUT input;
	//WORD vkey = vKey; // see link below
	//input.type = INPUT_KEYBOARD;
	//input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	//input.ki.time = 0;
	//input.ki.dwExtraInfo = 0;
	//input.ki.wVk = vkey;
	//input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	//SendInput(1, &input, sizeof(INPUT));

	keybd_event(vKey, MapVirtualKey(vKey, MAPVK_VK_TO_VSC), 0, 0);
	keybd_event(vKey, MapVirtualKey(vKey, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);

	//input.ki.dwFlags = KEYEVENTF_KEYUP;
	//SendInput(1, &input, sizeof(INPUT));

}

void Mouse::KeyboardClickDown(int vKey)
{
	INPUT input;
	WORD vkey = vKey; // see link below
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = vkey;
	input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	SendInput(1, &input, sizeof(INPUT));


}

void Mouse::KeyboardClickUp(int vKey)
{
	INPUT input;
	WORD vkey = vKey; // see link below
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = vkey;

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));

}