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
		_float4			vRotation = {0,0,0,1};
		_float3			vTranslation;
		_float				fTrackPosition;

		_bool IsBefore(_float nowTrackPosition) {
			return fTrackPosition < nowTrackPosition;
		}

		_XMKeyFrame LerpKeyFram(const tagKeyFrame& nextFrame, _float nowTrackPosition) {
			_XMKeyFrame lerpedFrame = {};
			_float fRatio = (nowTrackPosition - fTrackPosition) / (nextFrame.fTrackPosition - fTrackPosition);
			_float4 nextRot = nextFrame.vRotation;

			_vector nextRotation =XMLoadFloat4(&nextRot);
			_vector nowRotation =XMLoadFloat4(&vRotation);

			if (XMVector4Equal(nextRotation, XMVectorZero()))
						nextRotation = XMQuaternionIdentity();
			if (XMVector4Equal(nowRotation, XMVectorZero()))
				nowRotation = XMQuaternionIdentity();

			lerpedFrame.vScale = XMVectorLerp(XMLoadFloat3(&vScale), XMLoadFloat3(&nextFrame.vScale), fRatio);
			lerpedFrame.vRotation = XMQuaternionSlerp(nowRotation, nextRotation, (float)fRatio);
			lerpedFrame.vTranslation = XMVectorLerp(XMVectorSetW(XMLoadFloat3(&vTranslation), 1.f), XMVectorSetW(XMLoadFloat3(&nextFrame.vTranslation), 1.f), fRatio);
			return lerpedFrame;
		}

		_XMKeyFrame LerpKeyFram(const tagKeyFrame& nextFrame, _float nowTrackPosition, _float Distance) {
			_XMKeyFrame lerpedFrame = {};
			_float fRatio = nowTrackPosition / Distance;

			_float4 nextRot = nextFrame.vRotation;

			_vector nextRotation = XMLoadFloat4(&nextRot);
			_vector nowRotation = XMLoadFloat4(&vRotation);

			if (XMVector4Equal(nextRotation, XMVectorZero()))
				nextRotation = XMQuaternionIdentity();
			if (XMVector4Equal(nowRotation, XMVectorZero()))
				nowRotation = XMQuaternionIdentity();

			lerpedFrame.vScale = XMVectorLerp(XMLoadFloat3(&vScale), XMLoadFloat3(&nextFrame.vScale), fRatio);
			lerpedFrame.vRotation = XMQuaternionSlerp(nowRotation, nextRotation, (float)fRatio);
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

	typedef struct tagMinMaxBoxInfo {
		_float3 vMin = {};
		_float3 vMax = {};
	}MINMAX_BOX;

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

	typedef struct tagTileIndex {
		_int IndexX = { -1 };
		_int IndexZ = { -1 };
	}TILE_INDEX;

	typedef struct tagTileSystemInfo {
		/*몇개씩?*/
		_uint iTileCountX = {};
		_uint iTileCountZ = {};

		/*전체 크기*/
		_float3 vWorldMin = {};
		_float3 vWorldMax = {};
	public:
		_float3 SizePerTile() {
			return _float3{
				(vWorldMax.x - vWorldMin.x)/ iTileCountX,
				vWorldMax.y-vWorldMin.y,
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
		_bool Check_ValidIndex(TILE_INDEX index) {
			if (index.IndexX < 0 || index.IndexX >= static_cast<_int>(iTileCountX))
				return false;
			if (index.IndexZ < 0 || index.IndexZ >= static_cast<_int>(iTileCountZ))
				return false;

			return true;
		}

	}TILESYSTEM_INFO;


	struct TILE_INFO {
		_uint TileFlag = {};													//타일 타입 비트 플래그
		_float fCornerHeight[4] = {};
		class CTileBlock* pTileBlock = { nullptr };		//실제 배치된 타일
	};

	typedef struct tagMapFileHeader {
		_uint iObjectCount = {};
		_uint iTileCount = {};
	}MAP_FILE_HEADER;

	typedef struct tagMapObjectHeader {
		TILE_INDEX Index = {};
		_uint Object_type = {};
		_float4x4 vWorldMatrix = {};
		char ObjectName[MAX_PATH];
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
		string TypeName = {};
		AUTO_TILE_DESC rotateType[4];
		_bool Patial = {};
	}AUTO_TILE;

	typedef struct tagInstanceInitDESC {
		_uint instanceStride = {};		// 인스턴스 구조체 크기 
		_uint instanceCount = {};     // 최대 인스턴스 개수
		_uint ElementCount = {};
		const D3D11_INPUT_ELEMENT_DESC* pElementDesc = { nullptr };
		string ElementKey = {  };
	}INSTANCE_INIT_DESC;

	struct COLLIDER_SLOT
	{
		enum class STATE : _uint
		{
			NONE = 0,   // 비어 있음 (슬롯 미사용)
			ACTIVE = 1,   // 충돌 검사 대상
			INACTIVE = 2,   // 일시 비활성 (충돌 검사 제외)
			DEAD = 3,   // 소유자가 삭제됨, 완전히 제거 예정
		};

		class CCollider* pCollider;
		STATE eState = STATE::NONE;
		_uint iGeneration = {};

		bool IsValid() const { return eState != STATE::DEAD && pCollider != nullptr; }
		bool IsActive() const { return eState == STATE::ACTIVE && pCollider != nullptr; }
	};

	typedef struct tagCollisionInfoContext {
		string EventTag = {};
		class CGameObject* Owner = {nullptr};
	}COLLISION_CONTEXT;

#pragma pack(pop)

}


#endif // Engine_Struct_h__