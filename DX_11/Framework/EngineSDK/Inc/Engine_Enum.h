#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{

    enum class KEY_STATE {  NONE, TAP,HOLD, AWAY };

    enum class MOUSE_BTN  { LB,RB,MB};

    enum class WINMODE { FULL, WIN, END };

    enum class LEVEL_STATE{ INITIAL, REQUEST, LOADING, LOADED, STABLE };

}
#endif // Engine_Enum_h__
