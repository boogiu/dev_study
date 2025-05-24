#pragma once
#include <cmath>
typedef struct tagInfo {
	float fX; 
	float fY;
	float fCX;
	float fCY;
}INFO;


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
		return { (LONG)(fX + offsetRight),(LONG)(fY +offsetBottom) };
	}

	// Bottom을 기준으로 위쪽으로만 크기 조절
	void SetFromBottom(float startPoint) {
		float Size = this->Y_Size();
		fY = startPoint - (Size * 0.5f);
		offsetTop = (Size * 0.5f);
		offsetBottom = (Size * 0.5f);

	}

	// Top을 기준으로 아래쪽으로만 크기 조절
	void SetFromTop(float startPoint) {
		float Size = this->Y_Size();
		fY = startPoint + (Size * 0.5f);
		offsetTop = (Size * 0.5f);
		offsetBottom = (Size * 0.5f);
	}

	// Left를 기준으로 오른쪽으로만 크기 조절
	void SetFromLeft(float startPoint) {
		float Size = this->X_Size();
		fX = startPoint + (Size * 0.5f);
		offsetLeft = (Size * 0.5f);
		offsetRight = (Size * 0.5f);
	}

	// Right를 기준으로 왼쪽으로만 크기 조절
	void SetFromRight(float startPoint) {
		float Size = this->X_Size();
		fX = startPoint - (Size * 0.5f);
		offsetLeft = (Size * 0.5f);
		offsetRight = (Size * 0.5f);
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


typedef struct tagTileInfo {
	TILE_TYPE  eType;
	TILE_SPRITE tSprite;
	INFO_EX tInfo;

	void Set_FileKey(const TCHAR* imgKey) {
		wcscpy_s(tSprite.fileKey, imgKey);
	}
}TILE_INFO;

typedef struct tagLine {
	POINT LT;
	POINT RB;

	tagLine() : LT{}, RB{} {}
	tagLine(POINT _lt, POINT _rb) : LT(_lt), RB(_rb) {}
	void Set_Line(tagLine line){
		LT = line.LT;
		RB = line.RB;
	}

	// 1. 설정
	void Set(POINT _lt, POINT _rb) {
		LT = _lt;
		RB = _rb;
	}

	// 2. 길이 계산
	float Length() const {
		float dx = (float)(RB.x - LT.x);
		float dy = (float)(RB.y - LT.y);
		return std::sqrt(dx * dx + dy * dy);
	}

	// 3. 단위 방향 벡터
	POINT Direction() const {
		float len = Length();
		if (len == 0) return { 0, 0 };
		return {
			(LONG)((RB.x - LT.x) / len),
			(LONG)((RB.y - LT.y) / len)
		};
	}

	// 4. 중심 좌표
	POINT Center() const {
		return {
			(LT.x + RB.x) / 2,
			(LT.y + RB.y) / 2
		};
	}

	// 5. 벡터 형태 반환
	POINT Vector() const {
		return {
			RB.x - LT.x,
			RB.y - LT.y
		};
	}
	// 6. 점과의 거리 (수직 거리)
	float DistanceToPoint(POINT pt) const {
		float A = (float)(pt.x - LT.x);
		float B = (float)(pt.y - LT.y);
		float C = (float)(RB.x - LT.x);
		float D = (float)(RB.y - LT.y);

		float dot = A * C + B * D;
		float len_sq = C * C + D * D;
		float param = len_sq != 0 ? dot / len_sq : -1;

		float xx, yy;

		if (param < 0) {
			xx = (float)LT.x;
			yy = (float)LT.y;
		}
		else if (param > 1) {
			xx = (float)RB.x;
			yy = (float)RB.y;
		}
		else {
			xx = LT.x + param * C;
			yy = LT.y + param * D;
		}

		float dx = pt.x - xx;
		float dy = pt.y - yy;
		return sqrt(dx * dx + dy * dy);
	}
	float Get_Radian() const {
		return atan2f((float)(RB.y - LT.y), (float)(RB.x - LT.x));
	}

	float Get_Angle() const {
		return Get_Radian() * 180.f / PI;
	}

	// angleRad: 라디안, length: 선 길이
	void Set_By_Angle(float angleRad, float length = 100.f) {
		RB.x = (LONG)(LT.x + cosf(angleRad) * length);
		RB.y = (LONG)(LT.y + sinf(angleRad) * length);
	}

	void Rotate(float angleRad) {
		POINT center = Center();
		float dx = (float)(LT.x - center.x);
		float dy = (float)(LT.y - center.y);
		LT.x = (LONG)(center.x + dx * cosf(angleRad) - dy * sinf(angleRad));
		LT.y = (LONG)(center.y + dx * sinf(angleRad) + dy * cosf(angleRad));

		dx = (float)(RB.x - center.x);
		dy = (float)(RB.y - center.y);
		RB.x = (LONG)(center.x + dx *cosf(angleRad) - dy *sinf(angleRad));
		RB.y = (LONG)(center.y + dx *sinf(angleRad) + dy *cosf(angleRad));
	}

	bool Intersects(const tagLine& other) const {
		auto CCW = [](POINT a, POINT b, POINT c) -> int {
			int val = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
			if (val > 0) return 1;     // 반시계
			if (val < 0) return -1;    // 시계
			return 0;                  // 일직선
			};

		auto OnSegment = [](POINT p, POINT q, POINT r) -> bool {
			return min(p.x, r.x) <= q.x && q.x <= max(p.x, r.x) &&
				min(p.y, r.y) <= q.y && q.y <= max(p.y, r.y);
			};

		POINT A = LT;
		POINT B = RB;
		POINT C = other.LT;
		POINT D = other.RB;

		int ab_c = CCW(A, B, C);
		int ab_d = CCW(A, B, D);
		int cd_a = CCW(C, D, A);
		int cd_b = CCW(C, D, B);

		// 일반적인 교차
		if (ab_c * ab_d < 0 && cd_a * cd_b < 0) return true;

		// 특수한 경우: 선분이 일직선일 때 한 점이 다른 선분 위에 있음
		if (ab_c == 0 && OnSegment(A, C, B)) return true;
		if (ab_d == 0 && OnSegment(A, D, B)) return true;
		if (cd_a == 0 && OnSegment(C, A, D)) return true;
		if (cd_b == 0 && OnSegment(C, B, D)) return true;

		return false;
	}

} LINE_INFO;
