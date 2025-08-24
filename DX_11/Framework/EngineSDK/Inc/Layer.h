#pragma once
#include "Base.h"
NS_BEGIN(Engine)

class CLayer final :
    public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float dt);
	void Update(_float dt);
	void Late_Update(_float dt);

private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

NS_END