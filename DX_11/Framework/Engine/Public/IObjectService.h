#pragma once
#include "IService.h"

NS_BEGIN(Engine)

class ENGINE_DLL IObjectService : public IService
{
protected :
	~IObjectService() DEFAULT;
public:
	virtual void Engine_Update(_float dt) PURE;
	virtual void Priority_Update(_float fTimeDelta)PURE;
	virtual void Update(_float fTimeDelta)PURE;
	virtual void Late_Update(_float fTimeDelta)PURE;

	virtual void Add_Object(class CGameObject* object, const LAYER_DESC* layer)PURE;
	virtual const unordered_map<string, class CLayer*>& Get_LevelLayer(const string& LevelTag)PURE;

	virtual void Clear(const string& LevelTag)PURE;
	virtual HRESULT Sync_To_Level()PURE;

};

NS_END
