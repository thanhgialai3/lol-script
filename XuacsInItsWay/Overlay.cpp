#include "Overlay.h"

#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"

#include "Globals.h"
#include "Orbwalker.h"

#include "lua_functions.h"



bool g_MenuEnabled = false;

WNDPROC oriWndProc = NULL;


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Overlay::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);

	if (result && g_MenuEnabled)
	{
		return true;
	}


	switch (message)
	{
	case WM_SIZE:
		if (d3ddev != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			ImGui_ImplDX9_CreateDeviceObjects();
			break;
		}
	case WM_PAINT:
	{
		DwmExtendFrameIntoClientArea(hWnd, &margin);
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;


	case WM_CLOSE:
	{
		exit(0);
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

std::vector<luabridge::LuaRef> OnDrawCalls;

void Overlay::Create(HINSTANCE hInstance)
{

	RECT rc;


	HWND newhwnd = FindWindow(NULL, "League of Legends (TM) Client");
	if (newhwnd != NULL) {
		GetWindowRect(newhwnd, &rc);
		Screen_Width = rc.right - rc.left;
		Screen_Height = rc.bottom - rc.top;
	}
	else {
		ExitProcess(0);
	}

	margin.cyBottomHeight = Screen_Width;
	margin.cyTopHeight = Screen_Height;

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT,
		"WindowClass",
		"",
		WS_POPUP,
		0, 0,
		Screen_Width, Screen_Height,
		NULL,
		NULL,
		hInstance,
		NULL);



	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);


	ShowWindow(hWnd, SW_SHOW);


	Init(hWnd);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.DisplaySize = ImVec2(s_width, s_height);
	io.DisplaySize = ImVec2(400, 300);
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplDX9_Init(hWnd, d3ddev);
	ImGui_ImplDX9_CreateDeviceObjects();
	ImGui::StyleColorsDark();


	////

	////




	//////


	MSG msg;

	//::SetWindowPos(FindWindow(NULL, ""), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	//lua_State* lua_state = lua_open();
	//LuaKeeper::lua_list.push_back(lua_state);
	//luaL_openlibs(lua_state);

	//DefineFunctions(lua_state);

	//DefineClasses(lua_state);

	//DefineGlobals(lua_state);

	//int status = luaL_dofile(lua_state, "test.lua");


	while (TRUE)
	{
		::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		if (!FindWindow(NULL, "League of Legends (TM) Client"))
			ExitProcess(0);

		Render();


		//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, Screen_Width, Screen_Height, 0);

		g_MenuEnabled ? SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST) : SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			exit(0);

		Sleep(1);

	}
}

void Overlay::Init(HWND hwnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;  

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;     // set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = Screen_Width;  
	d3dpp.BackBufferHeight = Screen_Height;  

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateFont(d3ddev, 50, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &pFont);
}


void Menu()
{
	ImGui_ImplDX9_NewFrame();

	ImGui::Begin("");

	static int current_page = 1;

	if (ImGui::Button("Orbwalker"))
	{
		current_page = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("Drawing"))
	{
		current_page = 2;
	}
	ImGui::SameLine();

	if (ImGui::Button("Misc"))
	{
		current_page = 3;
	}


	if (current_page == 1)
	{
		CObject local_player = Management::local_player;
		for (int i = 0; i < Management::Player_List.size(); i++)
		{
			int temp_addr = Management::Player_List[i].basePosition;
			CObject player = Management::Manager.Read<CObject>(Management::Player_List[i].basePosition);
			player.basePosition = temp_addr;

			if (player.team == local_player.team) continue;

			char *nickname = new char[32];
			ReadProcessMemory(Management::Manager._process, (LPCVOID)(player.basePosition + 0x3158), nickname, 32, 0);
			
			std::string str = std::to_string(i) + " << Priority >> " + std::to_string(i);

			ImGui::Text(nickname);
			ImGui::SliderInt(str.c_str(), &Management::Priority_List[player.network_id], 1, 5);
		}



		if (ImGui::Button("Reset List"))
		{
			ReadEntities(true);
		}

		ImGui::SliderFloat(" <- Attack if health is under %", &Management::percent_health, 0, 100);


		ImGui::SliderFloat("<-- Attack Cast Delay Time Adjusting Percent", &Orbwalker::adjust_for_attack_cast_delay, 0, 150);
		ImGui::SliderFloat("<-- Attack Delay Time Adjusting Percent", &Orbwalker::adjust_for_attack_delay, 0, 150);

		LuaKeeper::mtx.lock();
		std::map<int, int> temp_map = Management::Priority_List;
		for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
		{
			luabridge::push(LuaKeeper::lua_list[i], temp_map);
			lua_setglobal(LuaKeeper::lua_list[i], "priority_list");

			luabridge::push(LuaKeeper::lua_list[i], Orbwalker::adjust_for_attack_cast_delay);
			lua_setglobal(LuaKeeper::lua_list[i], "attack_cast_delay_multiplier");

			luabridge::push(LuaKeeper::lua_list[i], Orbwalker::adjust_for_attack_delay);
			lua_setglobal(LuaKeeper::lua_list[i], "attack_delay_multiplier");

			luabridge::push(LuaKeeper::lua_list[i], Management::percent_health);
			lua_setglobal(LuaKeeper::lua_list[i], "attack_under_hp_percent");
		}
		LuaKeeper::mtx.unlock();

	}


	if (current_page == 2)
	{
		ImGui::Checkbox("Enabled", &Settings::drawings_enabled);
		//ImGui::Checkbox("Draw lines to heroes", &Settings::hero_line_draw);
	}

	ImGui::End();


	ImGui::EndFrame();
}


void Overlay::Render()
{
	g_MenuEnabled = GetKeyState(VK_F8);

	if (d3ddev != NULL)
	{		// clear the window alpha

		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		if (d3ddev->BeginScene() >= 0)
		{

		if (g_MenuEnabled)
			Menu();



		if (Settings::drawings_enabled)
		{
			LuaKeeper::mtx.lock();

			if (OnDrawCalls.size() <= 0)
			{
				for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
					OnDrawCalls.push_back(luabridge::getGlobal(LuaKeeper::lua_list[i], "OnDraw"));
			}

			for (int i = 0; i < OnDrawCalls.size(); i++)
			{
				if (OnDrawCalls[i].isFunction())
				{
					OnDrawCalls[i]();
				}

			}
			LuaKeeper::mtx.unlock();

		}

			

		//if(Settings::drawings_enabled)
		//{
		//	Management::local_player = CObject::GetLocalPlayer();

		//	Overlay::DrawCircle(Management::local_player.position, 24, Management::local_player.attack_range + 100, D3DCOLOR_ARGB(255, 255, 255, 255), 2);

		//	ReadEntities(false);
		//	CObject local_player = Management::local_player;

		//	if (Settings::hero_line_draw)
		//	{
		//		for (int i = 0; i < Management::Player_List.size(); i++)
		//		{
		//			CObject player = Management::Manager.Read<CObject>(Management::Player_List[i].basePosition);

		//			bool is_dead = deobfuscate<bool>(player.basePosition, 0x3E0);
		//			if (/*objArray[i].invulnerable || !isTargetable(objArray[i].basePosition) || !isAlive(objArray[i].basePosition) ||*/ player.visible == nonVisible || player.health <= 0.001f || player.max_health <= 100.0f || player.health == 1 || player.basePosition == Management::local_player.basePosition) continue;
		//			//if (!isAlive(player.basePosition))	continue;
		//			if (is_dead != 0) continue;

		//			D3DXVECTOR2 screen_pos = WorldToScreen(player.position);
		//			D3DXVECTOR2 screen_pos2 = WorldToScreen(local_player.position);
		//			if (screen_pos.x != 0 && screen_pos.y != 0)
		//			{
		//				Overlay::DrawLine(screen_pos.x, screen_pos.y, screen_pos2.x, screen_pos2.y, 1, true, D3DCOLOR_ARGB(255, 255, 255, 255));
		//			}
		//		}

		//	}


		//	if (Management::current_target.network_id != 0)
		//	{
		//		CObject object = GetEntityFromNetworkIdInArray(Management::current_target.network_id);
		//		if (object.health > 0 && local_player.health > 0)
		//		{
		//			Overlay::DrawCircle(object.position, 12, 100, D3DCOLOR_ARGB(255, 255, 255, 255), 1);
		//		}
		//	}

		//}


	
			if (g_MenuEnabled)
			{
				ImGui::Render();

				ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			}
			d3ddev->EndScene();
		}

		d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
	}
}


void Overlay::DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char *fmt)
{
	RECT FontPos = { x, y, x + 120, y + 16 };
	char buf[1024] = { '\0' };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);
	g_pFont->DrawText(NULL, buf, -1, &FontPos, DT_NOCLIP, color);
}


void Overlay::Setup(int screen_width, int screen_height) 
{
	Screen_Width = screen_width;
	Screen_Height = screen_height;
}

void Overlay::DrawCircle(D3DXVECTOR3 vPos, float flPoints, float flRadius, DWORD clrColor, float flThickness = 1.f)
{
	float flPoint = D3DX_PI * 2.0f / flPoints;

	for (float flAngle = 0; flAngle < (D3DX_PI * 2.0f); flAngle += flPoint)
	{
		D3DXVECTOR3 vStart(flRadius * cosf(flAngle) + vPos.x, flRadius * sinf(flAngle) + vPos.z, vPos.y);
		D3DXVECTOR3 vEnd(flRadius * cosf(flAngle + flPoint) + vPos.x, flRadius * sinf(flAngle + flPoint) + vPos.z, vPos.y);

		float z_temp = vStart.z;
		vStart.z = vStart.y;
		vStart.y = z_temp;

		z_temp = vEnd.z;
		vEnd.z = vEnd.y;
		vEnd.y = z_temp;

		D3DXVECTOR2 vStartScreen, vEndScreen;
		vStartScreen = WorldToScreen(vStart); 
		vEndScreen = WorldToScreen(vEnd);



		if (vStartScreen.x == 0 && vStartScreen.y == 0) break;
		if (vEndScreen.x == 0 && vEndScreen.y == 0) break;

		DrawLine(vStartScreen.x, vStartScreen.y, vEndScreen.x, vEndScreen.y, flThickness, true, clrColor);
	}
}

void Overlay::DrawLine(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color = D3DCOLOR_ARGB(255,255,255,255))
{
	ID3DXLine *m_Line;

	D3DXCreateLine(d3ddev, &m_Line);
	D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
	m_Line->SetWidth(width);
	m_Line->SetAntialias(1);
	m_Line->Begin();
	m_Line->Draw(line, 2, color);
	m_Line->End();
	m_Line->Release();
}

int Overlay::Screen_Width = 1360;
int Overlay::Screen_Height = 768;

LPDIRECT3D9 Overlay::d3d;
LPDIRECT3DDEVICE9 Overlay::d3ddev;
HWND Overlay::hWnd;
MARGINS Overlay::margin;
LPD3DXFONT Overlay::pFont;
LPDIRECT3DVERTEXBUFFER9 Overlay::g_pVB;
