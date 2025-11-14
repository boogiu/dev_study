#include "Client_Defines.h"
#include "PlayerState_Diy.h"

CPlayerState_Diy::CPlayerState_Diy()
{
}

HRESULT CPlayerState_Diy::OnEnter()
{
	return S_OK;
}

void CPlayerState_Diy::OnUpdate(_float dt)
{
}

HRESULT CPlayerState_Diy::OnExit()
{
	return S_OK;
}

CState* CPlayerState_Diy::HandleTransition()
{
	if (m_pPlayer->Get_InfoPack().isCrafting) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	if (m_pPlayer->Get_InfoPack().WorkBenchEncounter == false) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	return nullptr;
}

_uint CPlayerState_Diy::Get_InputMask() const
{
	return 0;
}

void CPlayerState_Diy::Render_State()
{
}


CPlayerState_Diy* CPlayerState_Diy::Create()
{
	return new CPlayerState_Diy;
}

void CPlayerState_Diy::Free()
{
	__super::Free();
}
