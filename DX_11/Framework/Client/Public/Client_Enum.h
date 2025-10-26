#pragma once

enum class TILE_FLAG : unsigned int {
    NONE = 0,
    FLAG_WALKABLE = 1 << 0,
    FLAG_BLOCKED = 1 << 1,
    FLAG_SWIMMABLE = 1 << 2,
    FLAG_DIGGABLE = 1 << 3,
    FLAG_TOOLINTERACT = 1 << 4,
    FLAG_TREE = 1 << 5,
    FLAG_STONE = 1 << 6,


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
enum class ITEM_TYPE {
    NONE, AXE,
};
