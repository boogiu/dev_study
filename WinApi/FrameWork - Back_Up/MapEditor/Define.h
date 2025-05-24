#pragma once

#define WINCX 1920
#define WINCY 1280

#define VK_MAX 256
#define CELLSIZE 64
#define CL_CELLSIZE 32

#define HOLLOW RGB(255,0,255)

enum class KEY_STATE {
	NONE,
	TAP,
	HOLD,
	AWAY
};


typedef struct tagINFO {
	float fX;
	float fY;
	float fCX;
	float fCY;
}INFO;


typedef struct tagTileInfo {
	RECT m_tSpriteRect;
	int m_Row;
	int m_Col;
	INFO m_tInfo;
	//wchar_t filePath[128]
}TILE_INFO;