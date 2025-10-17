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
#pragma pack(push,1)
	/*Model*/
	typedef struct ENGINE_DLL tagModelFileHeader {
		char ModelKey[MAX_PATH];
		_uint MeshCount = {};
		_bool isAnimate = { false };
	}MODEL_FILE_HEADER;

	typedef struct ENGINE_DLL tagMeshInfoHeader {
		char MeshName[MAX_PATH];
		_uint VerticesCount = {};
		_uint IndicesCount = {};
		_uint MaterialIndex = {};
		_uint BoneCount = {};
	}MESH_INFO_HEADER;

	typedef struct ENGINE_DLL tagSkeletonFileHeader{
		_uint BoneCount = {};
	}SKELETON_FILE_HEADER;

	typedef struct ENGINE_DLL tagBoneInfoHeader {
		char BoneName[MAX_PATH];
		_int ParentBoneIndex = {};
		_float4x4 TransformationMatrix = {};
	}BONE_INFO_HEADER;

	/*Material*/
	struct MaterialConstants
	{
		_float4 vMtrDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		_float4 vMtrlAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
		_float4 vMtrlSpecular = _float4(1.0f, 1.0f, 1.0f, 1.f);
		_float fSpecularPow = { 0.1f };
		_float3 vPadding;
	};

	typedef struct ENGINE_DLL tagMaterialFileHeader {
		_uint MaterialDataCount = {};
		char materialFileKey[MAX_PATH];
	}MATERIAL_FILE_HEADER;

	typedef struct ENGINE_DLL tagMaterialInfoHeader {
		_uint TextureTypeCount = {};
		char materialDataKey[MAX_PATH];
		char ShaderKey[MAX_PATH];
		char passConstant[MAX_PATH];
		MaterialConstants materialConstant = {};
	}MATERIAL_INFO_HEADER;

	typedef struct ENGINE_DLL tagTextuerFileHeader {
		_uint typeID = {};
		_uint TextureCount = {};
	}TEXTURE_FILE_HEADER;

	typedef struct ENGINE_DLL tagTextuerInfoHeader {
		char TextureKey[MAX_PATH];
	}TEXTURE_INFO_HEADER;

	/*Animation*/
	typedef struct ENGINE_DLL tagAnimationClipHeader {
		_bool					bLoop = { };
		_float					fDuration = {}; 
		_float					fTickPerSecond = {}; 
		_uint					iNumChannels = {};
		char					ClipName[MAX_PATH];
	}ANIMATION_CLIP_HEADER;

	typedef struct ENGINE_DLL tagAnimationChannelHeader {
		_bool				isRootBoneChannel = { false };
		_uint				iBoneIndex = {};
		_uint				iNumKeyFrames = {};
		char				BoneName[MAX_PATH];
	}ANIMATION_CHANNEL_HEADER;

	struct _XMKeyFrame {
		_vector vScale;
		_vector vRotation;
		_vector vTranslation;
	};

	typedef struct ENGINE_DLL tagKeyFrame
	{
		_float3			vScale;
		_float4			vRotation;
		_float3			vTranslation;
		_float				fTrackPosition;

		_bool IsBefore(_float nowTrackPosition) {
			return fTrackPosition < nowTrackPosition;
		}

		_XMKeyFrame LerpKeyFram(const tagKeyFrame& nextFrame, _float nowTrackPosition) {
			_XMKeyFrame lerpedFrame = {};
			_float fRatio = (nowTrackPosition - fTrackPosition) / (nextFrame.fTrackPosition - fTrackPosition);
			lerpedFrame.vScale = XMVectorLerp(XMLoadFloat3(&vScale), XMLoadFloat3(&nextFrame.vScale), fRatio);
			lerpedFrame.vRotation = XMQuaternionSlerp(XMLoadFloat4(&vRotation), XMLoadFloat4(&nextFrame.vRotation), fRatio);
			lerpedFrame.vTranslation = XMVectorLerp(XMVectorSetW(XMLoadFloat3(&vTranslation), 1.f), XMVectorSetW(XMLoadFloat3(&nextFrame.vTranslation), 1.f), fRatio);
			return lerpedFrame;
		}
		_XMKeyFrame LerpKeyFram(const tagKeyFrame& nextFrame, _float nowTrackPosition, _float Distance) {
			_XMKeyFrame lerpedFrame = {};
			_float fRatio = nowTrackPosition / Distance;
			lerpedFrame.vScale = XMVectorLerp(XMLoadFloat3(&vScale), XMLoadFloat3(&nextFrame.vScale), fRatio);
			lerpedFrame.vRotation = XMQuaternionSlerp(XMLoadFloat4(&vRotation), XMLoadFloat4(&nextFrame.vRotation), fRatio);
			lerpedFrame.vTranslation = XMVectorLerp(XMVectorSetW(XMLoadFloat3(&vTranslation), 1.f), XMVectorSetW(XMLoadFloat3(&nextFrame.vTranslation), 1.f), fRatio);
			return lerpedFrame;
		}

		tagKeyFrame& operator=(const tagKeyFrame& rhs)
		{
			if (this == &rhs) return *this;

			vScale = rhs.vScale;
			vRotation = rhs.vRotation;
			vTranslation = rhs.vTranslation;
			fTrackPosition = rhs.fTrackPosition;

			return *this;
		}
	}KEYFRAME;

	/*BoundingBox*/

	typedef struct tagBoundingBoxInfo {
		_float3 vMin = {};
		_float3 vMax = {};
	}BOUNDING_BOX;

	/*RayInfo*/
	typedef struct tagRayInfo {
		_float3 vRayOrigin = {};
		_float3 vRayDirection = {};
		_float fMaxDistance = {};
	}RAY;

	/*RayHitInfo*/
	typedef struct tagRayHitInfo {
		class CGameObject* pObject = { nullptr };
		_float fDistance = {};
		_float3 vHittedPosition = {};
	}RAY_HIT;

	/*Tile Grid System*/
	typedef struct tagTileSystemInfo {
		/*몇개씩?*/
		_uint iTileCountX = {};
		_uint iTileCountY = {};
		_uint iTileCountZ = {};

		/*전체 크기*/
		_float3 vWorldMin = {};
		_float3 vWorldMax = {};

	public:
		_float3 SizePerTile() {
			return _float3{
				(vWorldMax.x - vWorldMin.x)/ iTileCountX,
				(vWorldMax.y - vWorldMin.y)/ iTileCountY,
				(vWorldMax.z - vWorldMin.z)/ iTileCountZ
			};
		};

		_float3 HalfPoint() {
			return _float3{
				(vWorldMax.x + vWorldMin.x)*0.5f,
				(vWorldMax.y + vWorldMin.y)*0.5f,
				(vWorldMax.z + vWorldMin.z)*0.5f
			};
		};

		_float3 WorldSize() {
			return _float3{
				(vWorldMax.x - vWorldMin.x) ,
				(vWorldMax.y - vWorldMin.y) ,
				(vWorldMax.z - vWorldMin.z) 
			};
		};

	}TILESYSTEM_INFO;

	typedef struct tagTileIndex {
		_int IndexX = {-1};
		_int IndexY = {-1};
		_int IndexZ = {-1};
	}TILE_INDEX;

	struct TILE_INFO {
		_uint TileFlag = {};												//타일 타입 비트 플래그
		class CTileBlock* pTileBlock = { nullptr };		//실제 배치된 타일
	};

	typedef struct tagMapFileHeader {
		TILESYSTEM_INFO tileInfo = {};
		_uint iFieldOutCount = {};
		_uint iTileCount = {};
		_uint iStructureCount = {};
	}MAP_FILE_HEADER;

	typedef struct tagMapBaseHeader {
		TILESYSTEM_INFO tileInfo = {};
		_float4 vWorldPos = {};
		_float3 vWorldScale = {};
		char ModelName[MAX_PATH];
		char MaterialName[MAX_PATH];
		char ModelPath[MAX_PATH];
		char MaterialPath[MAX_PATH];
	}MAP_BASE_HEADER;

	typedef struct tagMapObjectHeader {
		TILE_INDEX Index = {};
		_float4 vWorldPos = {};
		char ModelName[MAX_PATH];
		char MaterialName[MAX_PATH];
		char ModelPath[MAX_PATH];
		char MaterialPath[MAX_PATH];
	}MAP_OBJECT_HEADER;

	typedef struct  tagMapTileHeader {
		_bool Is_Base = { false };
		_float fRotation = {};
		_uint CurState = {};
		TILE_INDEX Index = {};
		char BaseTypeName[MAX_PATH];
	}MAP_TILE_HEADER;

	typedef struct tagAutoTileFileHeader {
		_uint RuleCount = {};
	}AUTO_TILE_HEADER;

	typedef struct tagAutoTileDesc {
		_float rotation;                     
		NEIGHBOR_INDEX Connectable;         
		NEIGHBOR_INDEX NeverConnectable;     
	}AUTO_TILE_DESC;

	typedef struct tagAutoTile {

	}AUTO_TILE;

#pragma pack(pop)

	/* Input LayOut*/
	typedef struct ENGINE_DLL tagVertexPosition {
		XMFLOAT3		vPosition;

		static constexpr string_view  Key = "VTXPOS";
		static constexpr unsigned int iElementCount = { 1 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION",        0,      DXGI_FORMAT_R32G32B32_FLOAT,         0,      0,		D3D11_INPUT_PER_VERTEX_DATA,	0},
		};
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertexPositionColor {
		XMFLOAT3		vPosition;
		XMFLOAT4		vColor;

		static constexpr string_view  Key = "VTXCOL";
		static constexpr unsigned int iElementCount = { 2};
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION",        0,      DXGI_FORMAT_R32G32B32_FLOAT,         0,      0,		D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"COLOR",        0,      DXGI_FORMAT_R32G32B32A32_FLOAT,         0,     12,		D3D11_INPUT_PER_VERTEX_DATA,	0},
		};
	}VTXCOL;

	typedef struct ENGINE_DLL tagVertexTileDebuf {
		XMFLOAT3		vPosition;
		_int		iTileOffset;

		static constexpr string_view  Key = "VTXTILE";
		static constexpr unsigned int iElementCount = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION",        0,      DXGI_FORMAT_R32G32B32_FLOAT,         0,      0,		D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"TEXCOORD",        0,      DXGI_FORMAT_R32_SINT,         0,     12,		D3D11_INPUT_PER_VERTEX_DATA,	0},
		};
	}VTXTILE;

	typedef struct ENGINE_DLL tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static constexpr string_view  Key = "VTXPOSTEX";
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

		static constexpr string_view  Key = "VTXNORMTEX";
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

		static constexpr string_view  Key = "VTXMESH";
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

		static constexpr string_view  Key = "VTXSKINMESH";
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