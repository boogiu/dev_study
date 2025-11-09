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
	_int rand = Helper::Get_Random_Int(0, 5);

	switch (rand)
	{
	case 0:
		Animator->Change_Animation("Base_Wait.anim", false);
		break;
	case 1:
		Animator->Change_Animation("Act_Rhythm03.anim", false);
		break;
	case 2:
		Animator->Change_Animation("Base_Wait.anim", false);
		break;
	case 3:
		Animator->Change_Animation("Base_Wait.anim", false);
		break;
	case 4:
		Animator->Change_Animation("Base_Wait.anim", false);
		break;
	case 5:
		Animator->Change_Animation("Base_Wait.anim", false);
		break;
	default:
		break;
	}
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
