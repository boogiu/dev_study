#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{

    typedef struct tagEngineDesc {
        HWND hWnd;
        WINMODE			eWinMode;
        unsigned int	iWinSizeX, iWinSizeY;
    }ENGINE_DESC;

    /* Key Input struct*/
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

    typedef struct tagInitDesc {
        virtual ~tagInitDesc() DEFAULT;
    }INIT_DESC;

    /* Input LayOut*/
    typedef struct tagVertexPosition {
        XMFLOAT3		vPosition;
        static constexpr unsigned int iElementCount = { 1 };
        static const D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount];
    }VTXPOS;

    typedef struct tagVertexPostionTexcoord
    {
        XMFLOAT3		vPosition;
        XMFLOAT2		vTexcoord;
        static constexpr unsigned int iElementCount = { 2 };
        static const D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount];
    }VTXTEX;

}


#endif // Engine_Struct_h__
