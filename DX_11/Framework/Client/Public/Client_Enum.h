#pragma once

enum class TILE_FLAG : unsigned int {
    NONE = 0,
    FLAG_WALKABLE = 1 << 0,
    FLAG_BLOCKED = 1 << 1,
    FLAG_SWIMMABLE = 1 << 2,
    FLAG_DIGGABLE = 1 << 3,
    FLAG_SITTABLE = 1 << 4,

    FLAG_TREE = 1 << 6,
    FLAG_STONE = 1 << 7,


    FLAG_ONITEM= 1 << 30,
    ONPLAYER = 1 << 31,
};

inline TILE_FLAG operator | (TILE_FLAG a, TILE_FLAG b) {
    return static_cast<TILE_FLAG>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}
inline TILE_FLAG operator & (TILE_FLAG a, TILE_FLAG b) {
    return static_cast<TILE_FLAG>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}
inline TILE_FLAG operator & (TILE_FLAG a, unsigned int b) {
    return static_cast<TILE_FLAG>(static_cast<unsigned int>(a) & (b));
}
inline TILE_FLAG operator | (TILE_FLAG a, unsigned int b) {
    return static_cast<TILE_FLAG>(static_cast<unsigned int>(a) |(b));
}
inline _bool operator == (TILE_FLAG a, unsigned int b) {
    return static_cast<unsigned int>(a)== (b);
}
inline _bool operator != (TILE_FLAG a, unsigned int b) {
    return !(a == b);
}

const TILE_FLAG CANT_WALK = TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_SWIMMABLE | TILE_FLAG::FLAG_STONE | TILE_FLAG::FLAG_TREE;
const TILE_FLAG CANT_ITEM =   TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_SWIMMABLE | TILE_FLAG::FLAG_STONE | TILE_FLAG::FLAG_ONITEM;

enum class ITEM_TYPE {
    NONE, AXE,SCOOP,NET
};

enum class STATE_LAYER
{
    ACTION,
    TOOL,
    EMOTION,
    MAX
};