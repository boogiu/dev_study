#pragma once

#define WINCX 900
#define WINCY 600
#define MAP_CX 4500 
#define MAP_CY 2000
#define PURE = 0
#define VK_MAX 256
#define CELLSIZE 64
#define CL_CELLSIZE 48

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



typedef struct tagOffsetBox {
	float fX;
	float fY;

	float offsetLeft;
	float offsetRight;
	float offsetTop;
	float offsetBottom;

	tagOffsetBox()
		: fX(0), fY(0), offsetLeft(0), offsetRight(0), offsetTop(0), offsetBottom(0) {
	}

	tagOffsetBox(INFO info) { Translate_Info(info); }

	void Copy(tagOffsetBox info) {
		fX = info.fX;
		fY = info.fY;
		offsetLeft = info.offsetLeft;
		offsetRight = info.offsetRight;
		offsetTop = info.offsetTop;
		offsetBottom = info.offsetBottom;
	}

	void Translate_Info(INFO info) {
		fX = info.fX;
		fY = info.fY;
		offsetLeft = info.fCX * 0.5f;
		offsetRight = info.fCX * 0.5f;
		offsetTop = info.fCY * 0.5f;
		offsetBottom = info.fCY * 0.5f;
	}
	void Set_Pos(float _x, float _y) {
		fX = _x;
		fY = _y;
	}
	void Set_Pos(POINT _pt) {
		fX = _pt.x;
		fY = _pt.y;
	}
	POINT Get_Pos() {
		return{ (LONG)fX,(LONG)fY };
	}
	void Set_Size(float xSixe, float ySize) {
		offsetLeft = xSixe * 0.5f;
		offsetRight = xSixe * 0.5f;
		offsetTop = ySize * 0.5f;
		offsetBottom = ySize * 0.5f;
	};

	void Update_INFO(RECT& myRect) {
		myRect.left = static_cast<LONG>(fX - offsetLeft);
		myRect.right = static_cast<LONG>(fX + offsetRight);
		myRect.top = static_cast<LONG>(fY - offsetTop);
		myRect.bottom = static_cast<LONG>(fY + offsetBottom);
	}

	void Add_OffsetX(float left, float right) {
		offsetLeft += left;
		offsetRight += right;
	};
	void Add_OffsetY(float top, float bottom) {
		offsetTop += top;
		offsetBottom += bottom;
	};

	float X_Size() {
		return offsetLeft + offsetRight;
	}
	float Y_Size() {
		return offsetTop + offsetBottom;
	}
	POINT LT() {
		return { (LONG)(fX - offsetLeft),(LONG)(fY - offsetTop) };
	}
	POINT RB() {
		return { (LONG)(fX + offsetRight),(LONG)(fY + offsetBottom) };
	}
}COLLIDER_INFO, UI_INFO, INFO_EX;



typedef struct Tile_Sprite_Info {
	wchar_t fileKey[128];
	int m_Row;
	int m_Col;

	// 기본 생성자
	Tile_Sprite_Info()
		: m_Row(0), m_Col(0)
	{
		fileKey[0] = L'\0';
	}

	// 복사 생성자
	Tile_Sprite_Info(const Tile_Sprite_Info& other)
		: m_Row(other.m_Row), m_Col(other.m_Col)
	{
		wcscpy_s(fileKey, other.fileKey);
	}

	// 복사 대입 연산자
	Tile_Sprite_Info& operator=(const Tile_Sprite_Info& other)
	{
		if (this != &other)
		{
			m_Row = other.m_Row;
			m_Col = other.m_Col;
			wcscpy_s(fileKey, other.fileKey);
		}
		return *this;
	}

	// 비교 연산자 (같은 이미지, 같은 분할이라면 같다)
	bool operator==(const Tile_Sprite_Info& other) const
	{
		return m_Row == other.m_Row &&
			m_Col == other.m_Col &&
			wcscmp(fileKey, other.fileKey) == 0;
	}

} TILE_SPRITE;

enum TILE_TYPE { TILE_PLATFORM, TILE_OBJ, TILE_ORNAM, TILE_END };

typedef struct tagTileInfo {
	TILE_TYPE  eType;
	TILE_SPRITE tSprite;
	INFO_EX tInfo;

	void Set_FileKey(const TCHAR* imgKey) {
		wcscpy_s(tSprite.fileKey, imgKey);
	}
}TILE_INFO;
