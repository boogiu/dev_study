#pragma once
#include "State.h"
#include "PlayerHFSM.h"
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
	void Set_Player(class CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void Set_HFSM(class CPlayerHFSM* pHFSM) { m_pHFSM = pHFSM; }

protected:
	class CPlayer* m_pPlayer = { nullptr };
	class CPlayerHFSM* m_pHFSM= { nullptr };

public:
	virtual void Free();
};

NS_END