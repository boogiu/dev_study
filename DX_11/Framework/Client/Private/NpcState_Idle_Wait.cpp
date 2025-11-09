#include "Client_Defines.h"
#include "NpcState_Idle_Wait.h"
#include "NonPlayer.h"
#include "Animator3D.h"

CNpcState_Idle_Wait::CNpcState_Idle_Wait()
{
}

HRESULT CNpcState_Idle_Wait::OnEnter()
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	Animator->Change_Animation("Base_Wait.anim", false);
	return S_OK;
}

void CNpcState_Idle_Wait::OnUpdate(_float dt)
{
}

HRESULT CNpcState_Idle_Wait::OnExit()
{
	return S_OK;
}

CState* CNpcState_Idle_Wait::HandleTransition()
{
	return nullptr;
}

CNpcState_Idle_Wait* CNpcState_Idle_Wait::Create()
{
	return new CNpcState_Idle_Wait;
}
void CNpcState_Idle_Wait::Free()
{
	__super::Free();
}
