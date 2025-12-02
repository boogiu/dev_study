#pragma once

enum class TILE_FLAG : unsigned int {
    NONE = 0,
    /*Action*/
    FLAG_WALKABLE = 1 << 0,
    FLAG_BLOCKED = 1 << 1,
    FLAG_SWIMMABLE = 1 << 2,

    /**/
    FLAG_DIGGED= 1 << 4,

    FLAG_TILE = 1 << 5,
    FLAG_TREE = 1 << 6,
    FLAG_STONE = 1 << 7,
    FLAG_INSECT = 1 << 8,
    FLAG_STRUCTURE = 1 << 9,
    FLAG_GRASS = 1 << 10,
    FLAG_BRIDGE = 1 << 11,
    FLAG_FLOWER = 1 << 12,

    FLAG_RIVER = 1 << 13,
    FLAG_SEA = 1 << 14,
    FLAG_SAND = 1 << 15,
    FLAG_CLIFF = 1 << 16,

    /*ON*/
    FLAG_ONFURNITURE = 1 << 27,
    FLAG_ONITEM= 1 << 28,
    ONPLAYER= 1 << 29,
    ONCHARACTER = 1 << 30,
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

inline TILE_FLAG operator & (unsigned int b, TILE_FLAG a) {
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
/*겹치는 거 있다!*/
inline _bool operator && (TILE_FLAG a, TILE_FLAG b) {
    return (static_cast<unsigned int>(a) & static_cast<unsigned int>(b)) != 0;
}
inline _bool operator && (unsigned int a, TILE_FLAG b) {
    return (a & static_cast<unsigned int>(b)) != 0;
}
inline _bool operator && (TILE_FLAG a, unsigned int b) {
    return (b & static_cast<unsigned int>(a)) != 0;
}

const TILE_FLAG CANT_WALK =
TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_SWIMMABLE |TILE_FLAG::FLAG_CLIFF |
TILE_FLAG::FLAG_STONE | TILE_FLAG::FLAG_TREE | TILE_FLAG::FLAG_DIGGED | TILE_FLAG::FLAG_RIVER | TILE_FLAG::FLAG_SEA;

const TILE_FLAG CANT_DIG_REPELL =
TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_STONE ;

const TILE_FLAG CANT_DIG_AIR =
TILE_FLAG::FLAG_TILE | TILE_FLAG::FLAG_SWIMMABLE;

const TILE_FLAG CANT_ITEM =   TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_SWIMMABLE |
TILE_FLAG::FLAG_STONE | TILE_FLAG::FLAG_ONITEM;

enum class itemType { 
    None, 
    Fruit, 
    Ore, 
    Plant, 
    Insect,
    Fish,
    Axe,  
    Net, 
    Scoop,
    FishingRod,
    Represent,
    Furniture,
};

enum class STATE_LAYER
{
    ACTION,
    TOOL,
    EMOTION,
    MAX
};
enum class ActionPhase { Start, Proceed, End, None };


enum class InputMask : unsigned int {
        MOVE = 1 << 0,
        ACTION = 1 << 1,
        PICKUP = 1 << 2,
        BAG = 1 << 3,
        ADD = 1 << 4,
};       
inline constexpr  unsigned int operator & (InputMask a, InputMask b) {
    return (static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

inline constexpr  unsigned int operator & (unsigned int a, InputMask b) {
    return (a & static_cast<unsigned int>(b));
}

inline constexpr  unsigned int operator | (InputMask a, InputMask b) {
    return (static_cast<unsigned int>(a) | static_cast<unsigned int>(b)); 
}

inline constexpr  unsigned int operator | (unsigned int a, InputMask b) {
    return (a | static_cast<unsigned int>(b));  
}

inline constexpr  unsigned int operator | (InputMask a, unsigned int b) {
    return (b | static_cast<unsigned int>(a));  
}

inline _bool operator == (InputMask a, unsigned int b) {
    return static_cast<unsigned int>(a) == b;
}

inline _bool operator != (InputMask a, unsigned int b) {
    return !(a == b);
}

/*무브 스테이트에서 받을 수 없는 ㅆ는 입력*/
static  constexpr unsigned int FlagForMove = 0xffffffff;
static  constexpr unsigned int FlagForAction = InputMask::ACTION | InputMask::PICKUP;
static  constexpr unsigned int OnlyAction = 0xffffffff;
static  constexpr unsigned int OnlyTrans = 0xffffffff;
static  constexpr unsigned int OnlyInteraction = 0xffffffff;

enum class NpcState {
    Idle,
    Angry,
    Depressed,
    Happy
};

enum class GAME_MODE {
    NORMAL,
    RYTHM
};

