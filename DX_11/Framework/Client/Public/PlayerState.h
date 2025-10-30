#pragma once
#include "State.h"
#include "LayerState.h"
#include "PlayerStateMachine.h"
NS_BEGIN(Client)
class CPlayerState :
	public CState
{
protected:
	CPlayerState();
	virtual ~CPlayerState() DEFAULT;

public:
	virtual void OnEnter() {}
	virtual void OnUpdate(_float dt) {}
	virtual void OnExit() {}
	virtual CState* HandleTransition() { return nullptr; }

public:
	void Set_Owner(CGameObject* pObject) override;

public:
	virtual _uint Get_InputMask() const { return 0xFFFFFFFF; } 

protected:
	class CPlayer* m_pPlayer = { nullptr };

public:
	virtual void Free();
};

NS_END