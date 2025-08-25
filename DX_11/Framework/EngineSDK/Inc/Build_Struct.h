#pragma once
namespace Engine{
typedef struct tagInitDesc {
	virtual ~tagInitDesc() DEFAULT;
}INIT_DESC;

/**/
typedef struct tagGameObjectCloneDesc : public INIT_DESC {
	string OriginLevel;
	string protoTag;

	tagGameObjectCloneDesc(const string& _originLevel, const string& _protoTag) :protoTag(_protoTag),OriginLevel(_originLevel){}
	virtual ~tagGameObjectCloneDesc() DEFAULT;
}CLONE_DESC;

typedef struct LayerAddDesc : public INIT_DESC
{
	string DestLevel;
	string LayerTag;
	LayerAddDesc(const string& _destLevel, const string& _layerTag) :LayerTag(_layerTag), DestLevel(_destLevel) {}
	virtual ~LayerAddDesc() DEFAULT;
}LAYER_DESC;

/**/
typedef struct tagComponentDesc : public INIT_DESC {
	tagComponentDesc() {};
	virtual ~tagComponentDesc() DEFAULT;
}COMPONENT_DESC;

typedef struct TransformInitDesc : COMPONENT_DESC {
	_float3 vInitialPosition = {};
	_float3 vInitialEulerVector = {};
	_float3 vInitialScale = {1.f,1.f, 1.f};

	TransformInitDesc() DEFAULT;
	virtual ~TransformInitDesc() DEFAULT;
}TRANSFORM_DESC;

typedef struct CameraInitDesc : COMPONENT_DESC {
	
	_float3 vEye;

	CameraInitDesc() DEFAULT;
	virtual ~CameraInitDesc() DEFAULT;
}CAMERA_DESC;

typedef struct ColliderInitDesc : COMPONENT_DESC {

	ColliderInitDesc() DEFAULT;
	virtual ~ColliderInitDesc() DEFAULT;
}COLLIDER_DESC;
}