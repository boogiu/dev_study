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
		KEY_STATE state = KEY_STATE::Center;
	}KEY_DESC;

	typedef struct  tagMouseDesc
	{
		KEY_DESC mouseKey[3]; //0 L 1R 2M
		_float fDeltaX;
		_float fDeltaY;
		_float fWheelDelta;
	}MOUSE_DESC;

	/* Init Desc struct*/
	typedef struct tagInitDesc {
		virtual ~tagInitDesc() DEFAULT;
	}INIT_DESC;

	/* Light Desc struct*/
	typedef struct tagLightDesc {
		LIGHT_TYPE			eType = {};
		_float4		vDiffuse = {};
		_float4		vAmbient = {};
		_float4		vSpecular = {};

		_float4		vDirection = {};
		_float4		vPosition = {};
		_float			fRange = {};
	}LIGHT_DESC;

	/* Input LayOut*/
	typedef struct ENGINE_DLL tagVertexPosition {
		XMFLOAT3		vPosition;
		static constexpr unsigned int iElementCount = { 1 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION",        0,      DXGI_FORMAT_R32G32B32A32_FLOAT,         0,      0,		D3D11_INPUT_PER_VERTEX_DATA,	0},
		};
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static constexpr unsigned int					iElementCount = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static constexpr unsigned int					iElementCount = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXNORMTEX;
}


#endif // Engine_Struct_h__
