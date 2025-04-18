#pragma once

#define WINCX 800
#define WINCY 600

#define PURE =0

#define PI 3.141592

struct VECTOR2 {
    float fX;
    float fY;

public:
    VECTOR2() : fX(0), fY(0) {}
    VECTOR2(float _x, float _y) : fX(_x), fY(_y) {}

    VECTOR2(const VECTOR2& rhs) : fX(rhs.fX), fY(rhs.fY) {}
    VECTOR2(VECTOR2&& rhs) noexcept : fX(rhs.fX), fY(rhs.fY) {}
    
    VECTOR2& operator=(const VECTOR2& rhs) {
        if (this != &rhs) {
            fX = rhs.fX;
            fY = rhs.fY;
        }
        return *this;
    }
    
    VECTOR2 operator-(const VECTOR2& rhs) const {
        return VECTOR2(fX - rhs.fX, fY - rhs.fY);
    }

    VECTOR2 operator+(const VECTOR2& rhs) const {
        return VECTOR2(fX + rhs.fX, fY + rhs.fY);
    }
    VECTOR2& operator+=(const VECTOR2& rhs) {
        fX += rhs.fX;
        fY += rhs.fY;
        return *this;
    }

    VECTOR2& operator-=(const VECTOR2& rhs) {
        fX -= rhs.fX;
        fY -= rhs.fY;
        return *this;
    }

    bool operator ==(const VECTOR2& rhs) {
        return(fX == rhs.fX) && (fY == rhs.fY);
    }

    bool operator !=(const VECTOR2& rhs) {
        return !(*this == rhs);
    }
};

enum class KEY_STATE {
	TAP,
	HOLD,
	AWAY,
	NONE
};

enum class SCENE_ID {
	MAIN,
	STAGE,
	BOSS,
	END
};

enum class LAYER {

};

struct Velocity
{
	float fVX;
	float fVY;
};


template<typename T>
void Safe_Delete(T& ptr) {
	if (ptr) {
		delete ptr;
		ptr = nullptr;
	}
}

template<typename T>
class Singleton {
protected:
	Singleton() {};
	~Singleton() {};

public:
	Singleton(Singleton& rhs) = delete;
	Singleton& operator=(Singleton& rhs) = delete;

public:
	static T& GetInstance() {
		static T instance;
		return instance;
	}
};