#pragma once

#define WINCX 800
#define WINCY 600

#define PI 3.1414592
#define PURE =0

extern HWND g_hWnd;
extern RECT mouseRange;

template<typename T>
void Safe_Delete(T& _ptr) {
	if (_ptr) {
		delete _ptr;
		_ptr = nullptr;
	}
}

template<typename T>
class Singleton {

protected:
	Singleton() {}
	~Singleton() {}

public:
	Singleton(const Singleton& rhs) = delete;
	Singleton& operator = (const Singleton& rhs) = delete;

	static T& GetInstance() {
		static T instance;
		return instance;
	}
};

enum class SCENE {
	MAIN,
	STAGE,
	END
};

enum class OBJ_LAYER {
	PLAYER,
	ENEMY,
	BULLET,
	END
};


enum class GRD_LAYER {
	BORDER,
	PLATFORM,
	END
};


enum class PHYSICS {
	COLLIDER,
	GRAVITY,
	VELOCITY,
	ALL,
	END
};

typedef struct  tagINFO
{
	float fX;
	float fY;

	float fCX;
	float fCY;
} INFO;

typedef struct tagVelocity {
	float fVX;
	float fVY;
}VELOCITY;

enum  class KEY_STATE {
	NONE,
	TAP,
	HOLD,
	AWAY,
};

constexpr short sDEAD		= 1	<<	0;
constexpr short sIDLE		= 1	<<	1;
constexpr short sMOVE	= 1	<<	2;
constexpr short sJUMP		= 1	<<	3;
constexpr short sONHIT	= 1	<<	4;

