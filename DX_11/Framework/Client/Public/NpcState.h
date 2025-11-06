#pragma once
#include "State.h"
#include "LayerState.h"
#include "NpcState_Machine.h"

NS_BEGIN(Client)
class CNpcState :
    public CState
{
protected:
	CNpcState();
	virtual ~CNpcState() DEFAULT;

public:
	virtual HRESULT OnEnter() { return S_OK; }
	virtual void OnUpdate(_float dt) {}
	virtual HRESULT OnExit() { return S_OK; }
	virtual CState* HandleTransition() { return nullptr; }

public:
	void Set_Owner(CGameObject* pObject) override;

public:
	virtual _uint Get_InputMask() const { return 0xFFFFFFFF; }

protected:
	class CNonPlayer* m_pCharacter = { nullptr };

public:
	virtual void Free();
};

NS_END