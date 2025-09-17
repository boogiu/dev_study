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
		_float			fRange = {};
	}LIGHT_DESC;

	/*File Info Desc*/
	/*Model*/
	typedef struct ENGINE_DLL tagMeshFileHeader {
		char meshKey[64];
		_uint MeshCount = {};
	}MESH_FILE_HEADER;

	typedef struct ENGINE_DLL tagMeshInfoHeader {
		_bool isAnimate = { false };
		_uint VerticesCount = {};
		_uint IndicesCount = {};
		_uint MaterialIndex = {};
		_uint BoneCount = {};
	}MESH_INFO_HEADER;

	typedef struct ENGINE_DLL tagSkeletonFileHeader{
		_uint BoneCount = {};
	}SKELETON_FILE_HEADER;

	typedef struct ENGINE_DLL tagBoneInfoHeader {
		char BoneName[64];
		_uint ParentBoneIndex = {};
		_float4x4 TransformationMatrix = {};
		_float4x4 CombinedTransformationMatrix = {};

	}BONE_INFO_HEADER;

	/*Mateial*/
	struct MaterialConstants
	{
		_float4 vMtrDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		_float4 vMtrlAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
		_float4 vMtrlSpecular = _float4(1.0f, 1.0f, 1.0f, 1.f);
		_float fSpecularPow = { 0.1f };
		_float3 vPadding;
	};

	typedef struct ENGINE_DLL tagMaterialFileHeader {
		char materialDataKey[64];
		_uint MaterialDataCount = {};
	}MATERIAL_FILE_HEADER;

	typedef struct ENGINE_DLL tagMaterialInfoHeader {
		MaterialConstants materialConstant = {};
		char passConstant[64];
		char ShaderKey[64];
		_uint TextureTypeCount = {};
	}MATERIAL_INFO_HEADER;

	typedef struct ENGINE_DLL tagTextuerFileHeader {
		_uint typeID = {};
		_uint TextureCount = {};
	}TEXTURE_FILE_HEADER;

	typedef struct ENGINE_DLL tagTextuerInfoHeader {
		char TextureKey[64];
	}TEXTURE_INFO_HEADER;

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

	typedef struct ENGINE_DLL tagVertexMeshPosNorTexTan
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static constexpr unsigned int					iElementCount = {4 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXMESH;

	typedef struct ENGINE_DLL tagVertexSkinnedMeshPosNorTexTan
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		XMUINT4 vBlendIndex;
		XMFLOAT4 vBlendWeight;

		static constexpr unsigned int					iElementCount = { 6 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTXSKINMESH;
}


#endif // Engine_Struct_h__