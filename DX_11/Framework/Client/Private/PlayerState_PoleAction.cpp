#include "Client_Defines.h"
#include "PlayerState_PoleAction.h"
#include "Animator3D.h"

CPlayerState_PoleAction::CPlayerState_PoleAction()
{
}

HRESULT CPlayerState_PoleAction::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();

	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.Range_FowardInfo.TileFlag;

	HRESULT hr;

	if ((TILE_FLAG::FLAG_RIVER & Flag) != 0) {
		hr = Animator->Change_Animation("ToolPole_Swing.anim");
	}
	else {
		hr = Animator->Change_Animation("ToolPole_Air.anim", true);
	}

	return S_OK;
}

void CPlayerState_PoleAction::OnUpdate(_float dt)
{
}

HRESULT CPlayerState_PoleAction::OnExit()
{
	return E_NOTIMPL;
}

CState* CPlayerState_PoleAction::HandleTransition()
{
	return nullptr;
}

void CPlayerState_PoleAction::Render_State()
{
}

_uint CPlayerState_PoleAction::Get_InputMask() const
{
	return _uint();
}

CPlayerState_PoleAction* CPlayerState_PoleAction::Create()
{
	return new CPlayerState_PoleAction;
}
void CPlayerState_PoleAction::Free()
{
	__super::Free();
}