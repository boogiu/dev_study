#pragma once

#include <windows.h>
#include <d3d11.h>

#include "Engine_Defines.h"

namespace MapEditor {
	static constexpr unsigned int g_iWinSizeX = 1920;
	static constexpr unsigned int g_iWinSizeY = 960;
	//static constexpr unsigned int g_iWinSizeX = 1280;
	//static constexpr unsigned int g_iWinSizeY = 720;
	static constexpr unsigned int g_iMainFrame = 144;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

using namespace MapEditor;

enum Mode {NONE, EditObj, BrushTile, TileSystem };
enum ModelMap {ID, Name, ModleFileName, ModelFileName, ModelFilePath, MaterialFilePath,END};
enum class TILE_FLAG : unsigned int {
    NONE = 0,
    FLAG_WALKABLE = 1 << 0,
    FLAG_BLOCKED = 1 << 1,
    FLAG_SWIMMABLE = 1 << 2,
};