#pragma once

#include <windows.h>
#include <d3d9.h>
#include <iostream>
#include <d3dx9.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>

#pragma comment(lib, "dwmapi")


#ifndef OVERLAY_H

#define OVERLAY_H

struct vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

enum circle_type { full, half, quarter };
enum text_alignment { lefted, centered, righted };



static class Overlay {
public:
	static void Create(HINSTANCE hInstance);


	static void Init(HWND hwnd);
	static void Render();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void Setup(int screen_width, int screen_height);


	static void DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char *fmt);
	static void DrawLine(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color);
	static void DrawCircle(D3DXVECTOR3 vPos, float flPoints, float flRadius, DWORD clrColor, float flThickness);



	static int Screen_Width;
	static int Screen_Height;

	static LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
	static LPDIRECT3DDEVICE9 d3ddev;
	static HWND hWnd;
	static MARGINS  margin;
	static LPD3DXFONT pFont;
	static LPDIRECT3DVERTEXBUFFER9 g_pVB;

};


#endif // !MENU_H
