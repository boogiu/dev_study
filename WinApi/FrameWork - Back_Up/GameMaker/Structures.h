#pragma once

typedef struct tagInfo {
	float fX; 
	float fY;
	float fCX;
	float fCY;
}INFO;

typedef struct tagLinePoint {
	float fX;
	float fY;

	tagLinePoint() {
		ZeroMemory(this, sizeof(this));
	}
	tagLinePoint(float _x, float _y) :fX(_x), fY(_y) {}
}LINEPOINT;

typedef struct tagLine {
	tagLinePoint tLPoint;
	tagLinePoint tRPoint;

	tagLine() {
		ZeroMemory(this, sizeof(this));
	}
	tagLine(LINEPOINT tLeft, LINEPOINT tRight) : tLPoint(tLeft), tRPoint(tRight) {}
};

typedef struct tagVelocity {
	float fX;
	float fY;

	tagVelocity() {
		ZeroMemory(this, sizeof(this));
	}

	tagVelocity(float _x, float _y) {
		fX = _x; fY = _y;
	}

	bool operator ==(tagVelocity& rhs) {
		return (rhs.fX == fX && rhs.fY == fY);
	}
}VECTOR2, VELOCITY;

typedef struct tagTileInfoToLoad {
	RECT m_tSpriteRect;
	int m_Row;
	int m_Col;
	INFO m_tInfo;
}TILE_INFO_LOAD;

typedef struct tagTileInfo {
	int m_Row;
	int m_Col;
}TILE_INFO;


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

}COLLIDER_INFO, UI_INFO,INFO_EX;


typedef struct tagFrame {
	POINT startPoint;
	POINT endPoint;
	POINT m_spSize;

	float frameSpeed;
	
	bool isLoop;
	POINT LoopPoint;
} ANI_FRAME;
