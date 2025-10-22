#pragma once

enum class TILE_FLAG : unsigned int {
    NONE = 0,
    ONPLAYER = 1 << 0,
    WALKABLE = 1 << 1,
    DIGGABLE = 1 << 2,
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
