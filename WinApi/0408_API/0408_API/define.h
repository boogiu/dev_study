#pragma once

#define WINCX 800
#define WINCY 600

#define PI 3.141592
#define PURE =0

template<typename T>
void Safe_Delete(T& p) {
	if (p) {
		delete p;
		p = nullptr;
	}
}

//사용할 키 종류
enum class KEY
{
	NONE	=		1 << 0,
	UP			=		1 << 1,
	DOWN	=		1 << 2,
	LEFT		=		1 << 3,
	RIGHT	=		1 << 4,
	W			=		1 << 5,
	A				=		1 << 6,
	S				=		1 << 7,
	D				=		1 << 8,
	SPACE	=		1 << 9
};


constexpr short sNoneDir	=		0x0000; //정지;
constexpr short sUpDir			=		0x0001 ; // 위
constexpr short sDownDir	=		0x0010 ; //  아래
constexpr short sLeftDir		=		0x0100 ; //  왼쪽
constexpr short sRightDir	=		0x1000 ; //  오른쪽

enum KEYSTATE
{
	NONE = 0x0000,
	TAP = 0x8000,
	HOLD = 0x8001,
	AWAY = 0x0001
};

//물체의 중심좌표와 사이즈
typedef struct  tagInfo
{
	//중심
	float fX;
	float fY;
	//크기
	float fCX;
	float fCY;
}INFO;

//입력된 키의 상태 : 키 종류 / 키 상태
typedef struct tagKey {
	int KeyFlag;
	KEYSTATE eState;
}KEYINFO;

extern HWND g_hWnd;