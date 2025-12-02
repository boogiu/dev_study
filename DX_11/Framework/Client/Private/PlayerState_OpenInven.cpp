#include "Client_Defines.h"
#include "PlayerState_OpenInven.h"
#include "Animator3D.h"
#include "GameInstance.h"

CPlayerState_OpenInven::CPlayerState_OpenInven()
{
}

HRESULT CPlayerState_OpenInven::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();
	m_pPlayer->Adjust_To_WorldFoward();
	m_pPlayer->Camera_Zoom_In();
	m_pPlayer->Open_Inventory();
	Animator->Change_Animation("Menu_Think.anim");
	
	return S_OK;
}

void CPlayerState_OpenInven::OnUpdate(_float dt)
{
}

HRESULT CPlayerState_OpenInven::OnExit()
{
	m_pPlayer->Close_Inventory();
	m_pPlayer->Get_ControlPack().MsgBag = false;
	m_pPlayer->Camera_Restore();

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	return S_OK;
}

CState* CPlayerState_OpenInven::HandleTransition()
{
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap('I')) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	return nullptr;
}

void CPlayerState_OpenInven::Render_State()
{
}

_uint CPlayerState_OpenInven::Get_InputMask() const
{
	return (1 << static_cast<_uint>(InputMask::BAG));
}

CPlayerState_OpenInven* CPlayerState_OpenInven::Create()
{
	return new CPlayerState_OpenInven;
}

void CPlayerState_OpenInven::Free()
{
	__super::Free();
}
