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
	void Priority_Update(_float dt);
	void Update(_float dt);
	void Late_Update(_float dt);

public:
	/*{level,proto}*/
	class CBuilder Create_Object(const CLONE_DESC clone);
	void Add_Object(class CGameObject* object , const LAYER_DESC* layer);
	void Clear(const string& LevelTag);
	HRESULT Sync_To_Level();

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	unordered_map<string, LAYERS> m_Layers; //(레벨 태그 / (레이어 태그/레이어))

public:
	static CObjectMgr* Create();
	virtual void Free() override;
};
NS_END
