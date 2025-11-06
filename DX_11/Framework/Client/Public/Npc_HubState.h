#pragma once
#include "HubState.h"
NS_BEGIN(Client)
class CNpc_HubState :
    public CHubState
{
protected:
	CNpc_HubState();
	virtual ~CNpc_HubState() DEFAULT;

public:
	virtual HRESULT OnEnter() { return S_OK; }
	virtual void OnUpdate(_float dt) {}
	virtual HRESULT OnExit() { return S_OK; }
	virtual CState* HandleTransition() { return nullptr; }

protected:
	virtual class CNpcState* Add_State(class CNpcState* pState, string stateName);

public:
	void DecideSubState(_float dt) PURE;
	void Set_Owner(CGameObject* pObject) override;
	
protected:
	class CNonPlayer* m_pCharacter = { nullptr };

public:
	virtual void Free() override;
};

NS_END