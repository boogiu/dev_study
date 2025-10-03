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
		_uint				iBoneIndex = {};
		_uint				iNumKeyFrames = {};
		char			BoneName[MAX_PATH];
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

	/*Tile Grid System Info*/
	typedef struct tagTileSystemInfo {
		_float4 OriginPoint = {}; //그리드 원점
		/*몇개씩?*/
		_uint iTileCountX = {};
		_uint iTileCountY = {};
		_uint iTileCountZ = {};
		/*사이즈?*/
		_uint iSizeXPerTile = {};
		_uint iSizeYPerTile = {};
		_uint iSizeZPerTile = {};

	}TILESYSTEM_INFO;
#pragma pack(pop)

	/* Input LayOut*/
	typedef struct ENGINE_DLL tagVertexPosition {
		XMFLOAT3		vPosition;

		static constexpr string_view  Key = "VTXPOS";
		static constexpr unsigned int iElementCount = { 1 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iElementCount] = {
			{"POSITION",        0,      DXGI_FORMAT_R32G32B32A32_FLOAT,         0,      0,		D3D11_INPUT_PER_VERTEX_DATA,	0},
		};
	}VTXPOS;

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