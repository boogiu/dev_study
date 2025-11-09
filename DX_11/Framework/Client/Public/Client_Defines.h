#pragma once

#include <windows.h>
#include <d3d11.h>


namespace Client {
	static constexpr unsigned int g_iWinSizeX = 1280;
	static constexpr unsigned int g_iWinSizeY = 720;
	static constexpr unsigned int g_iMainFrame = 144;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

using namespace Client;

#include "Engine_Defines.h"
#include "Client_Enum.h"
#include "Client_Structs.h"
#include "Event_Structs.h"

constexpr Engine::TILE_INDEX NEIGHBOR_OFFSET[9] = {
	{-1, +1}, // LEFT_TOP
	{ 0, +1}, // TOP
	{+1, +1}, // TOP_RIGHT
	{-1,  0}, // LEFT
	{ 0,  0}, // CENTER
	{+1,  0}, // RIGHT
	{+1, -1}, // RIGHT_BOTTOM
	{ 0, -1}, // BOTTOM
	{-1, -1}, // BOTTOM_LEFT
};

static constexpr _float4 Ibory = { 1.0f, 0.984f, 0.905f ,1.f };