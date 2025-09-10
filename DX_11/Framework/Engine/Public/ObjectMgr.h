#pragma once
#include "IObjectService.h"


NS_BEGIN(Engine)

using LAYERS = unordered_map<string, class CLayer*>;

class CObjectMgr final : public IObjectService 
{
private:
	CObjectMgr();
	~CObjectMgr() DEFAULT;
	HRESULT Initialize();
public:
	virtual void Engine_Update(_float dt) override;
	virtual void Priority_Update(_float dt) override;
	virtual void Update(_float dt) override;
	virtual void Late_Update(_float dt) override;

public:
	/*{level,proto}*/
	virtual void Add_Object(class CGameObject* object , const LAYER_DESC* layer) override;
	virtual void Clear(const string& LevelTag) override;
	virtual HRESULT Sync_To_Level() override;
	virtual const unordered_map<string, class CLayer*>& Get_LevelLayer(const string& LevelTag) override;
private:
	class CGameInstance* m_pGameInstance = { nullptr };
	unordered_map<string, LAYERS> m_Layers; //(레벨 태그 / (레이어 태그/레이어))

public:
	static CObjectMgr* Create();
	virtual void Free() override;
};
NS_END
