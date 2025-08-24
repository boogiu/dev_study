#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
    typedef struct tagKeyDesc {
        _bool PrevDown = false;
        _bool CurrDown = false;
        KEY_STATE state = KEY_STATE::NONE;
    }KEY_DESC;

    typedef struct  tagMouseDesc
    {
        KEY_DESC mouseKey[3] ; //0 L 1R 2M
        _float fDeltaX;
        _float fDeltaY;
        _float fWheelDelta;
    }MOUSE_DESC;

    typedef struct tagEngineDesc {
        HWND hWnd;
        WINMODE			eWinMode;
        unsigned int	iWinSizeX, iWinSizeY;
    }ENGINE_DESC;

}


#endif // Engine_Struct_h__
