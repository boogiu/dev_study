#pragma once

#define WINCX 800
#define WINCY 600

#define PURE =0
/*
*1. 중점 X 
*2. 중점 Y 
*3. 크기 X
*4. 크기 Y
*/
typedef struct tagINFO
{
	float fX;
	float fY;
	float fCX;
	float fCY;
} INFO;

template<typename T>
void Safe_Delete(T& ptr)
{
	if (ptr) {
		delete ptr;
		ptr = nullptr;
	}
}


//////열거형 zone

enum KEY_STATE {
	NONE,
	TAP,
	HOLD,
	AWAY
};