#include "Client_Defines.h"
#include "NpcState_Idle_LookAround.h"

#include "NonPlayer.h"
#include "Animator3D.h"
#include "Helper_Func.h"

CNpcState_Idle_LookAround::CNpcState_Idle_LookAround()
{
}

HRESULT CNpcState_Idle_LookAround::OnEnter()
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	Animator->Change_Animation("Base_Wait.anim", false);

	return S_OK;
}

void CNpcState_Idle_LookAround::OnUpdate(_float dt)
{
}

HRESULT CNpcState_Idle_LookAround::OnExit()
{
	return S_OK;
}

CState* CNpcState_Idle_LookAround::HandleTransition()
{
	return nullptr;
}

CNpcState_Idle_LookAround* CNpcState_Idle_LookAround::Create()
{
	return new CNpcState_Idle_LookAround;
}
void CNpcState_Idle_LookAround::Free()
{
	__super::Free();
}
