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
    /*Action*/
    FLAG_WALKABLE = 1 << 0,
    FLAG_BLOCKED = 1 << 1,
    FLAG_SWIMMABLE = 1 << 2,

    /**/
    FLAG_DIGGED = 1 << 4,

    FLAG_TILE = 1 << 5,
    FLAG_TREE = 1 << 6,
    FLAG_STONE = 1 << 7,
    FLAG_INSECT = 1 << 8,
    FLAG_STRUCTURE = 1 << 9,
    FLAG_GRASS = 1 << 10,
    FLAG_BRIDGE = 1 << 11,

    FLAG_RIVER = 1 << 12,
    FLAG_SEA = 1 << 13,

    /*ON*/
    FLAG_ONFURNITURE = 1 << 27,
    FLAG_ONITEM = 1 << 28,
    ONPLAYER = 1 << 29,
    ONCHARACTER = 1 << 30,
};