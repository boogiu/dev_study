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




typedef struct tagTileInfo {
	RECT m_tSpriteRect;
	int m_Row;
	int m_Col;
	INFO m_tInfo;
	//wchar_t filePath[128]
}TILE_INFO;