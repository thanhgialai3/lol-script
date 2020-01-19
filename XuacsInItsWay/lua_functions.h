#pragma once


#ifndef LUA_FUNCTIONS_H
#define LUA_FUNCTIONS_H

#include "Mouse.h"
#include "Globals.h"

#include "Overlay.h"


D3DXVECTOR3* checkVec3(lua_State* L, int index);


D3DXVECTOR2* checkVec2(lua_State* L, int index);

static int read_missiles_func(lua_State *L)
{
	ReadMissiles(true);

	//luabridge::push

	/* return the number of results */
	return 0;
}

static int read_entities_func(lua_State *L)
{
	ReadEntities(true);

	//luabridge::push

	/* return the number of results */
	return 0;
}

static int read_heroes_func(lua_State *L)
{
	ReadHeroes(false);
	/* return the number of results */
	return 0;
}


static int log_print(lua_State *L)
{
	/* get number of arguments */

	if (lua_isnumber(L,1))
	{
		int num = lua_tonumber(L, 1);

		std::cout << num << std::endl;
	}
	else if (lua_isstring(L, 1))
	{
		std::string str = lua_tostring(L, 1);

		std::cout << str.c_str() << std::endl;
	}

	/* return the number of results */
	return 0;
}


static int sleep_func(lua_State *L)
{
	/* get number of arguments */


	int millisecond = lua_tointeger(L, 1);

	Sleep(millisecond);

	/* return the number of results */
	return 0;
}

static int time_now(lua_State* L)
{
	
	lua_pushinteger(L, GetTickCount());

	return 1;
}


static int script_click(lua_State *L)
{
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);

	Mouse::ScriptClick(x, y);

	/* return the number of results */
	return 0;
}

static int right_click(lua_State *L)
{
	Mouse::RightClick();

	/* return the number of results */
	return 0;
}


static int left_click(lua_State *L)
{
	Mouse::LeftClick();

	/* return the number of results */
	return 0;
}


static int move_mouse(lua_State *L)
{
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);

	Mouse::Move(x, y);

	/* return the number of results */
	return 0;
}

static int keyboard_click(lua_State *L)
{
	char key = (char)lua_tonumber(L, 1);

	Mouse::KeyboardClick(key);

	/* return the number of results */
	return 0;
}





static int is_key_pressed(lua_State* L)
{
	char key = (char)lua_tonumber(L, 1);

	lua_pushboolean(L, GetAsyncKeyState(key));

	return 1;
}



static int WorldToScreenLUA(lua_State* L)
{
	float x = lua_tonumber(L, 1), y = lua_tonumber(L, 2), z = lua_tonumber(L, 3);
	
	D3DXVECTOR2 screen_pos = WorldToScreen(D3DXVECTOR3(x, y, z));

	luabridge::push(L, screen_pos);

	return 1;
}


inline float ReadFloat(int address)
{
	return Management::Manager.Read<float>(address);
}


inline int ReadInt(int address)
{
	return Management::Manager.Read<int>(address);
}


inline short ReadShort(int address)
{
	return Management::Manager.Read<short>(address);
}


inline std::string ReadString(int address, int len)
{
	char* pointer = (char*)Management::Manager.ReadBytes(address, len);

	std::string str(pointer);

	return str;
}


inline void DrawLine( D3DXVECTOR2 start_pos, D3DXVECTOR2 end_pos )
{
	Overlay::DrawLine(start_pos.x, start_pos.y, end_pos.x, end_pos.y, 1, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
}
inline void DrawWhiteLine(D3DXVECTOR2 start_pos, D3DXVECTOR2 end_pos)
{
	Overlay::DrawLine(start_pos.x, start_pos.y, end_pos.x, end_pos.y, 1, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
}

inline void DrawRedLine(D3DXVECTOR2 start_pos, D3DXVECTOR2 end_pos)
{
	Overlay::DrawLine(start_pos.x, start_pos.y, end_pos.x, end_pos.y, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 0));
}

inline void DrawCircle(D3DXVECTOR3 pos, int smoothness, float radius)
{
	Overlay::DrawCircle(pos, smoothness, radius, D3DCOLOR_ARGB(255, 255, 255, 255), 1);
}


static int read_local_player(lua_State* L)
{


	Management::local_player = CObject::GetLocalPlayer();
	Management::under_mouse_object = CObject::GetUnderMouseObject();

	luabridge::push(L, Management::local_player);
	lua_setglobal(L, "myHero");

	luabridge::push(L, Management::under_mouse_object);
	lua_setglobal(L, "underMouseObject");
	//ReadEntities(false);
	return 0;
}



void DefineClasses(lua_State* index);


void DefineFunctions(lua_State* index);


void DefineGlobals(lua_State* index);




#endif