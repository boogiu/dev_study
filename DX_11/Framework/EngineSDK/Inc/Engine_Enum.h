#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{

    enum class KEY_STATE {  Center, TAP,HOLD, AWAY };

    enum class MOUSE_BTN  { LB,RB,MB};

    enum class WINMODE { FULL, WIN, END };

    enum class LEVEL_STATE{ INITIAL, REQUEST, LOADING, LOADED, STABLE };

    enum class STATE { RIGHT, UP, LOOK, POSITION, END };

    enum class BUFFER_TYPE { 
        BASIC_RECT , BASIC_CUBE, BASIC_SPHERE,
        MESH, TERRAIN,
    };

    enum class SOUND_GROUP{BGM, SFX, UI, TALK,END };

    enum class RENDER_PASS{ RENDER_PRIORITY,  RENDER_OPAQUE, RENDER_TRANSPARENT, RENDER_UI, RG_END };

    /*Align to Pivot*/
    enum class UI_Anchor : unsigned int {
        Center = 0,
        Left = 1,				// 0001
        Right = 2,			// 0010
        Top = 4,			// 0100
        Bottom = 8		// 1000
    };

    enum class LIGHT_TYPE{DIRECTIONAL, POINT,SPOTLIGHT};
}
#endif // Engine_Enum_h__
