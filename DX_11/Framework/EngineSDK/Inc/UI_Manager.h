#pragma once
#include "IUI_Service.h"
NS_BEGIN(Engine)
using UIobjects = vector<class CUI_Object*>;

class CUI_Manager :
	public IUI_Service
{
private:
	CUI_Manager();
	virtual ~CUI_Manager();
public:
	virtual void Post_EngineUpdate(_float dt) override;
	virtual void Priority_Update(_float dt) override;
	virtual void Update(_float dt) override;
	virtual void Late_Update(_float dt) override;
public:
	virtual void Clear(const string& LevelTag) override;
	virtual HRESULT Sync_To_Level() override;
	virtual void Add_UIObject(class CUI_Object* object, const string& level) override;
private:
	class CGameInstance* m_pGameInstance = { nullptr };
	unordered_map<string, UIobjects> m_UIObjects;

public:
	static CUI_Manager* Create();
	virtual void Free() override;
};
NS_END
