#include <Windows.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <ios>



////////

#include "Globals.h"

#include "Orbwalker.h"

#include "Mouse.h"

#include "Overlay.h"

///////

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include <LuaBridge/LuaBridge.h>

//////

#include "lua_functions.h"

/////





///////

void RedirectIOToConsole() {

	//Create a console for this application
	AllocConsole();

	// Get STDOUT handle
	HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
	FILE *COutputHandle = _fdopen(SystemOutput, "w");

	// Get STDERR handle
	HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
	int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
	FILE *CErrorHandle = _fdopen(SystemError, "w");

	// Get STDIN handle
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
	FILE *CInputHandle = _fdopen(SystemInput, "r");

	//make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	std::ios::sync_with_stdio(true);

	// Redirect the CRT standard input, output, and error handles to the console
	freopen_s(&CInputHandle, "CONIN$", "r", stdin);
	freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
	freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

	//Clear the error state for each of the C++ standard stream objects. We need to do this, as
	//attempts to access the standard streams before they refer to a valid target will cause the
	//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
	//to always occur during startup regardless of whether anything has been read from or written to
	//the console or not.
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();

}


///////




bool should_refresh = true, pause = false;



void report_errors(lua_State *L, int status)
{
	if (status != 0)
	{
		printf("-- %s\n", lua_tostring(L, -1));
		lua_pop(L, 1); // remove error message
	}
}



void main_thread()
{
	std::vector<luabridge::LuaRef> loop_funcs;

	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)read_entities_thread, 0, 0, 0);


	Sleep(3000);

	while (true)
	{
		if (pause) continue;

		if (should_refresh)
		{
			loop_funcs.clear();

			for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
				loop_funcs.push_back(luabridge::getGlobal(LuaKeeper::lua_list[i], "Loop"));

			should_refresh = false;
		}


		for (int i = 0; i < loop_funcs.size(); i++)
		{
			loop_funcs[i]();
		}

		//Orbwalker::Update();

		Sleep(1);
	}
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) 
{
	RedirectIOToConsole();


	while (!Management::Manager.Attached)
	{
		Management::Manager.Attach("League of Legends.exe");
		Sleep(100);
	}

	while (Management::LolClient.dwBase <= 0)
	{
		Management::LolClient = Management::Manager.GetModule("League of Legends.exe");
		Sleep(100);
	}

	CRenderer renderer = Management::Manager.Read<CRenderer>(Management::Manager.Read<DWORD>(Management::LolClient.dwBase + offsets::m_RendererClass));

	while (renderer.ScreenResolutionX <= 0 || renderer.ScreenResolutionY <= 0)
	{
		renderer = Management::Manager.Read<CRenderer>(Management::Manager.Read<DWORD>(Management::LolClient.dwBase + offsets::m_RendererClass));

		Sleep(100);
	}


	while (Management::Manager.Read<int>( Management::LolClient.dwBase + offsets::m_LocalPlayer ) <= 0)
	{
		Sleep(100);
	}

	Sleep(1000);


	std::string cmd_line(lpCmdLine);

	std::vector<std::string> parameters = split(cmd_line, " ");

	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main_thread, 0, 0, 0); // Orbwalker



	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)read_entities_thread, 0, 0, 0);

	for (int i = 0; i < parameters.size(); i++)
	{
		lua_State* lua_state = lua_open();
		LuaKeeper::lua_list.push_back(lua_state);
		luaL_openlibs(lua_state);

		DefineFunctions(lua_state);

		DefineClasses(lua_state);

		ReadEntities(true);
		read_local_player(lua_state);

		std::cout << "loading " << parameters[i].c_str() << "..." << std::endl;
		// Run the script
		int status = luaL_dofile(lua_state, parameters[i].c_str());

		report_errors(lua_state, status);

		DefineGlobals(lua_state);
	}


	std::vector< luabridge::LuaRef > loop_functions;

	for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
	{
		loop_functions.push_back(luabridge::getGlobal(LuaKeeper::lua_list[i], "Loop"));
	}


	Overlay::Setup(renderer.ScreenResolutionX, renderer.ScreenResolutionY);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Overlay::Create, hInstance, 0, 0);

	while (true)
	{
		LuaKeeper::mtx.lock();
		for (int i = 0; i < loop_functions.size(); i++)
			loop_functions[i]();
		LuaKeeper::mtx.unlock();

		Sleep(1);
	}


    return 0;
}

