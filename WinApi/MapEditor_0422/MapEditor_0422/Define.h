#pragma once

#define WINCX		800
#define WINCY		600

enum TILE_ID { TILE_NONE,TILE_WALL, TILE_THORN, TILE_INVWALL, TILE_END };
enum DIRECTION
{
	LEFT,
	UP,
	RIGHT,
	DOWN,
	END
};

typedef struct tagTile {
	RECT tRect;
	TILE_ID eType;
	DIRECTION eDir;

	bool operator==(const tagTile& rhs) const {
		return (tRect.left == rhs.tRect.left &&
			tRect.top == rhs.tRect.top &&
			tRect.right == rhs.tRect.right &&
			tRect.bottom == rhs.tRect.bottom);
	}

}TILE;