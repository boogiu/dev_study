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
	void Engine_Update(_float dt);
	void Priority_Update(_float dt);
	void Update(_float dt);
	void Late_Update(_float dt);

	class CGameObject* Find_ObjectByID(_uint ID);
	const vector<CGameObject*>& Get_AllObject() { return m_GameObjects; };

private:
	vector<class CGameObject*>			m_GameObjects; /*¼øÈ¸¿ë*/
	unordered_map<_uint,class CGameObject*>			m_GameObjectsByID;

public:
	static CLayer* Create();
	virtual void Free() override;
};

NS_END