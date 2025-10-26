#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{

    enum class KEY_STATE { Center, TAP, HOLD, AWAY };

    enum class MOUSE_BTN { LB, RB, MB };

    enum class WINMODE { FULL, WIN, END };

    enum class LEVEL_STATE { INITIAL, REQUEST, LOADING, LOADED, STABLE };

    enum class STATE { RIGHT, UP, LOOK, POSITION, END };

    enum class BUFFER_TYPE {
        BASIC_RECT, BASIC_CUBE, BASIC_SPHERE, BASIC_PLANE, TERRAIN, MESH
    };
    enum class MESH_TYPE  {ANIM,  NONANIM   };
    enum class COLLIDER_TYPE  {AABB, OBB, SPHERE};

    enum class SOUND_GROUP { BGM, SFX, UI, TALK, END };

    enum class RENDER_PASS { RENDER_PRIORITY, RENDER_OPAQUE, RENDER_TRANSPARENT, RENDER_UI, RG_END };

    enum class ANCHOR : unsigned int {
        Center = 0,
        Left = 1,				// 0001
        Right = 2,			// 0010
        Top = 4,			// 0100
        Bottom = 8		// 1000
    };

    inline ANCHOR operator | (ANCHOR a, ANCHOR b) {
        return static_cast<ANCHOR>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    }
    inline ANCHOR operator & (ANCHOR a, ANCHOR b) {
        return static_cast<ANCHOR>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
    }

    enum class NEIGHBOR_INDEX : unsigned int {
        UPLEFT = 1<<0, 
        UP = 1 << 1,
        UPRIGHT = 1 << 2,
        LEFT = 1 << 3,
        CENTER = 1 << 4,
        RIGHT = 1 << 5,
        DOWNLEFT = 1 <<6,
        DOWN = 1 << 7,
        DOWNRIGHT = 1 << 8,
        END =0
    };

    inline NEIGHBOR_INDEX operator | (NEIGHBOR_INDEX a, NEIGHBOR_INDEX b) {
        return static_cast<NEIGHBOR_INDEX>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    }
    inline NEIGHBOR_INDEX operator & (NEIGHBOR_INDEX a, NEIGHBOR_INDEX b) {
        return static_cast<NEIGHBOR_INDEX>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
    }

    enum class LIGHT_TYPE { DIRECTIONAL, POINT, SPOTLIGHT };

    enum class TEXTURE_TYPE {
        NONE = 0,
        ALBEDO = 1,
        NORMAL = 2,
        OPACITY = 3,
        EMMISION =4,

        ALBEDO_GRAY = 5,
        NORMAL_ORY= 6,
        ALBEDO_ORY = 7,
        EMMISION_ORY =8,
        INDEXMAP=9,
        SCALEX = 10,
        SCALEY = 11,
        SCALEXY = 12,
        
        GRADATION = 13,
        GRADATION_EDGE = 14,
        MIX = 15,
        SPECULAR = 16,
        END = 17
    };

    enum class TILE_NEIGHBOR { 
        CENTET, RIGHT, RIGHT_BOTTOM,BOTTOM, BOTTOM_LEFT, LEFT, LEFT_TOP,TOP,TOP_RIGHT,END
    };
}
#endif // Engine_Enum_h__


