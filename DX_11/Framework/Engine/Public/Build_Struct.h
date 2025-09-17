#pragma once
namespace Engine {
	/*Base Clone Desc*/
	typedef struct tagGameObjectCloneDesc : public INIT_DESC {
		string OriginLevel;
		string protoTag;

		tagGameObjectCloneDesc(const string& _originLevel, const string& _protoTag) :protoTag(_protoTag), OriginLevel(_originLevel) {}
		tagGameObjectCloneDesc() {};
		virtual ~tagGameObjectCloneDesc() DEFAULT;
	}CLONE_DESC;

	typedef struct LayerAddDesc : public INIT_DESC
	{
		string DestLevel;
		string LayerTag;
		LayerAddDesc(const string& _destLevel, const string& _layerTag) :LayerTag(_layerTag), DestLevel(_destLevel) {}
		virtual ~LayerAddDesc() DEFAULT;
	}LAYER_DESC;

	/*Component Clone Desc*/
	typedef struct tagComponentDesc : public INIT_DESC {
		tagComponentDesc() {};
		virtual ~tagComponentDesc() DEFAULT;
	}COMPONENT_DESC;

	typedef struct TransformInitDesc :public COMPONENT_DESC {
		_float3 vInitialPosition = {};
		_float3 vInitialEulerVector = {};
		_float3 vInitialScale = { 1.f,1.f, 1.f };

		TransformInitDesc() DEFAULT;
		virtual ~TransformInitDesc() DEFAULT;
	}TRANSFORM_DESC;

	typedef struct CameraInitDesc :public COMPONENT_DESC {
		_float fNear = {0.1f};
		_float fFar = {1000};
		_float fAspect = {};
		_float fFov = {60.f};
		CameraInitDesc() DEFAULT;
		CameraInitDesc(_float fFar, _float fNear, _float fAspect) : fNear{ fNear }, fFar{ fFar }, fAspect{ fAspect } {};
		CameraInitDesc(_float fAspect) : fAspect{ fAspect } {};
		virtual ~CameraInitDesc() DEFAULT;
	}CAMERA_DESC;

	typedef struct LightInitDesc :public COMPONENT_DESC {

		LIGHT_TYPE			eType = {};
		_float4		vDiffuse = {};
		_float4		vAmbient = {};
		_float4		vSpecular = {};

		_float4		vDirection = {};
		_float4		vPosition = {};
		_float			fRange = {};


		LightInitDesc() DEFAULT;
		virtual ~LightInitDesc() DEFAULT;

	}LIGHT_INIT_DESC;

	typedef struct ColliderInitDesc :public COMPONENT_DESC {

		ColliderInitDesc() DEFAULT;
		virtual ~ColliderInitDesc() DEFAULT;
	}COLLIDER_DESC;

	/*Object Clone Desc*/
	typedef struct tagGameObjectDesc : public INIT_DESC {
		string InstanceName = "";
		unordered_map<type_index, COMPONENT_DESC*> CompDesc;

		virtual ~tagGameObjectDesc() DEFAULT;
	}GAMEOBJECT_DESC;

	typedef struct tagUIObjectDesc : public GAMEOBJECT_DESC {
		_float fX = {};
		_float fY = {  };
		_float fSizeX = {1.f };
		_float fSizeY = { 1.f };
		_float fRadian = {};
		tagUIObjectDesc() DEFAULT;
		tagUIObjectDesc(const _float4& ui_Info) :fX(ui_Info.x), fY(ui_Info.y), fSizeX(ui_Info.z), fSizeY(ui_Info.w), GAMEOBJECT_DESC{} {}
		virtual ~tagUIObjectDesc() DEFAULT;
	}UI_DESC;
	

}