#include "Client_Defines.h"
#include "PlayerState_Run.h"
#include  "Player.h"
#include "Animator3D.h"
#include "Transform.h"
#include "GameInstance.h"
#include "IInputService.h"
#include "AudioSource.h"

CPlayerState_Run::CPlayerState_Run()
{
}

HRESULT CPlayerState_Run::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->Change_Animation("Move_Dash_F.anim", false, 0.05);
	auto AudioSource = m_pPlayer->Get_Component<CAudioSource>();
	AudioSource->Play("Dash_Grass");

     m_bFliping = false;
	m_fDuration = 0;
	return hr;
}

void CPlayerState_Run::OnUpdate(_float dt)
{
	m_fDuration += dt;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
	CPlayer::MovementPacket& tMovePacket = m_pPlayer->Get_MovementPacket();
	auto AudioSource = m_pPlayer->Get_Component<CAudioSource>();

	_float MoveSpeed = tMovePacket.fMoveSpeed;
	_float2 Player_InputAxis = tMovePacket.vInputAxis;

	if (!tMovePacket.bFliping && !m_bFliping) {
		_float2 myAxis = {};
		myAxis.x = Player_InputAxis.x * MoveSpeed * 1.5f * dt;
		myAxis.y = Player_InputAxis.y * MoveSpeed * 1.5f * dt;

		m_pPlayer->Can_Walk(myAxis);
		CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
		pTransform->Translate({ myAxis.x ,tMovePacket.fPlayerHeight * MoveSpeed * 1.5f* dt,myAxis.y });
		m_vLastAxis = Player_InputAxis;
	}

	else {
		if (m_fDuration > 1.5f) {
			if (!m_bFliping)
				Animator->Change_Animation("MoveTurn_Dash_L.anim", false);

			m_bFliping = true;
			_float2 breakAxis = {};

			breakAxis.x = MoveSpeed * 1.5f * m_vLastAxis.x * dt;
			breakAxis.y = MoveSpeed * 1.5f * m_vLastAxis.y * dt;

			m_pPlayer->Can_Walk(breakAxis);
			CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
			pTransform->Translate({ breakAxis.x ,tMovePacket.fPlayerHeight * MoveSpeed * 1.5f * dt,breakAxis.y });

			float damping = powf(0.98f, dt * 60.f);
			m_vLastAxis.x *= damping;
			m_vLastAxis.y *= damping;
			if (Animator->isCurrentAnimEnd()) {
				Animator->Change_Animation("Move_Dash_F.anim", false, 0.2);
				m_bFliping = false;
				m_fDuration = 0.f;
			}
		}
		else {
		}
	}

	m_fRunTime += dt;
	m_fRunSoundTime += dt;
	auto tTilePack = m_pPlayer->Get_TileInfoPacket();

	if (m_fRunSoundTime > 0.45f) {

		if ((tTilePack.nowInfo.TileFlag & TILE_FLAG::FLAG_SAND) != 0)
			AudioSource->Play("Dash_Sand");
		else
			AudioSource->Play("Dash_Grass");

		m_fRunSoundTime = 0.f;
	}

	if (m_fRunTime > 0.7f) {
		Request_Dust();
		m_fRunTime = 0.f;
	}

	auto tileSys = CGameInstance::GetInstance()->Get_TileSystem();
	_uint TileFlag = tileSys->Get_TileFlagByIndex(m_pPlayer->Get_TileInfoPacket().nowIndex);
	if ((TileFlag & TILE_FLAG::FLAG_FLOWER)!=0) {
		Request_Flower();
	}
	m_prevIndex = m_pPlayer->Get_TileInfoPacket().nowIndex;
}

HRESULT CPlayerState_Run::OnExit()
{
	m_bFliping = false;
	m_fRunTime = 0.f;
	return S_OK;
}

CState* CPlayerState_Run::HandleTransition()
{
	CPlayer::MovementPacket& tMovePacket = m_pPlayer->Get_MovementPacket();
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();
	_float2 InputAxis = tMovePacket.vInputAxis;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (m_bFliping)
		return nullptr;
	if (!control.MsgMove)
	{
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	if (!control.MsgAdd)
	{
		return m_pLayer->Get_State("Movement_Walk_State");
	}
	
	return nullptr;
}

void CPlayerState_Run::Render_State()
{
	ImGui::Text("Last Axis : X : %.1f, Z : %.1f", m_vLastAxis.x, m_vLastAxis.y);
}

void CPlayerState_Run::Request_Dust()
{
	EffectData data;
	data.ReqPosition = m_pPlayer->Get_Position();
	data.FxPosition = m_pPlayer->Get_Position();
	m_pPlayer->Request_Effect("Effect_Dust", data);
}

void CPlayerState_Run::Request_Flower()
{
	if (m_prevIndex.isSame(m_pPlayer->Get_TileInfoPacket().nowIndex)) {
		return;
	}
	else {
		EffectData data;
		data.ReqPosition = m_pPlayer->Get_Position();
		data.FxPosition = m_pPlayer->Get_Position();
		m_pPlayer->Request_Effect("Effect_Flower", data);

	}
}

_uint CPlayerState_Run::Get_InputMask() const
{
	return FlagForMove;
}


CPlayerState_Run* CPlayerState_Run::Create()
{
	return new CPlayerState_Run;
}

void CPlayerState_Run::Free()
{
	__super::Free();
}
