#include "Client_Defines.h"
#include "PlayerState_Walk.h"
#include  "Player.h"
#include "Animator3D.h"
#include "Transform.h"
#include "GameInstance.h"
#include "IInputService.h"
#include "AudioSource.h"
CPlayerState_Walk::CPlayerState_Walk()
{
}

HRESULT CPlayerState_Walk::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->Change_Animation("Move_Run_F.anim");

	return hr;
}

void CPlayerState_Walk::OnUpdate(_float dt)
{
	CPlayer::MovementPacket tMovePacket = m_pPlayer->Get_MovementPacket();

	auto AudioSource = m_pPlayer->Get_Component<CAudioSource>();
	if (!tMovePacket.bFliping) {
		_float2 myAxis = {};

		myAxis.x = tMovePacket.vInputAxis.x * tMovePacket.fMoveSpeed * dt;
		myAxis.y = tMovePacket.vInputAxis.y * tMovePacket.fMoveSpeed * dt;

		if (m_pPlayer->Can_Walk(myAxis)) {
			CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
			pTransform->Translate({ myAxis.x ,tMovePacket.fPlayerHeight * tMovePacket.fMoveSpeed * dt,myAxis.y });
		}
	}

	m_fWalkTime += dt;
	m_fWalkSoundTime += dt;
	auto tTilePack = m_pPlayer->Get_TileInfoPacket();

	if (m_fWalkSoundTime > 0.35f) {
		
		if ((tTilePack.nowInfo.TileFlag &TILE_FLAG::FLAG_SAND) != 0)
			AudioSource->Play("Run_Sand_L");
		else
			AudioSource->Play("Run_Grass_L");

		m_fWalkSoundTime = 0.f;
	}
	if (m_fWalkTime > 0.8f) {
		Request_Dust();
		m_fWalkTime = 0.f;
	}
}

HRESULT CPlayerState_Walk::OnExit()
{
	m_fWalkTime = 0.f;
	m_fWalkSoundTime = 0.35f; 
	return S_OK;
}

CState* CPlayerState_Walk::HandleTransition()
{
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (false == control.MsgMove) {
		Animator->Change_Animation("ToStop_RunLatter_L.anim",0.05f);
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	else if (m_pPlayer->Get_ControlPack().MsgAction && m_pPlayer->Get_ItemPacket().CurItem.TypeTag == itemType::Net) {
		return m_pLayer->Get_State("Movement_AimWalk_State");
	}
	else if (control.MsgAdd&& control.MsgMove) {
		return m_pLayer->Get_State("Movement_Run_State");
	}
	return nullptr;
}

_uint CPlayerState_Walk::Get_InputMask() const
{
	return FlagForMove;
}

void CPlayerState_Walk::Request_Dust()
{
	EffectData data;
	data.ReqPosition = m_pPlayer->Get_Position();
	data.FxPosition = m_pPlayer->Get_Position();
	m_pPlayer->Request_Effect("Effect_Dust", data);
}

CPlayerState_Walk* CPlayerState_Walk::Create()
{
	return new CPlayerState_Walk;
}

void CPlayerState_Walk::Free()
{
}
