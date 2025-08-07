#pragma once

#include <windows.h>
#include <d3d11.h>


namespace Client {
	static constexpr unsigned int g_iWinSizeX = 1280;
	static constexpr unsigned int g_iWinSizeY = 720;
	static constexpr unsigned int g_iMainFrame = 60;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

using namespace Client;

