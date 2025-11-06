#include "Client_Defines.h"
#include "NpcState_Move.h"
#include "NonPlayer.h"
#include "Animator3D.h"

CNpcState_Move::CNpcState_Move()
{
}

HRESULT CNpcState_Move::OnEnter()
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->Change_Animation("Move_Walk_L.anim", true);
	if (FAILED(hr)) {
		return hr;
	}
	return hr;
}
void CNpcState_Move::OnUpdate(_float dt)
{
	//m_pCharacter->Ch

	m_pCharacter->Get_Component<CTransform>()->Translate({ 8 * dt,0,0, });
}

HRESULT CNpcState_Move::OnExit()
{
	return S_OK;
}

CState* CNpcState_Move::HandleTransition()
{
	return nullptr;
}

CNpcState_Move* CNpcState_Move::Create()
{
	return new CNpcState_Move;
}

void CNpcState_Move::Free()
{
	__super::Free();
}
