#include "Client_Defines.h"
#include "PlayerState_Dig.h"
#include "PlayerStateMachine.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "Builder.h"

CPlayerState_Dig::CPlayerState_Dig()
{
}

HRESULT CPlayerState_Dig::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();

	m_DigComplete = false;

	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.Range_FowardInfo.TileFlag;

	HRESULT hr; 

	if ((TILE_FLAG::FLAG_DIGGED & Flag) != 0) {
		hr=Animator->Change_Animation("ToolScoop_BuryHole.anim");
		TILE_INDEX forward = m_pPlayer->Get_FowardIndex();
		_float4 pos = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(forward, ANCHOR::Center);
		pos.y += 0.5f;
		m_pPlayer->Request_Effect("Effect_BurryingGround", { pos, pos });

		m_isDigged = true;
	}
	else if ((TILE_FLAG::FLAG_TREE & Flag) != 0) {
		hr = Animator->Change_Animation("ToolScoop_DigStump.anim");
		m_isTree = true;
	}
	else if ((TILE_FLAG::FLAG_STONE & Flag) != 0) {
		hr = Animator->Change_Animation("ToolScoop_Repelled.anim");
		m_isStone = true;
	}
	else if ((CANT_DIG_REPELL & Flag) != 0)
	{
		hr = Animator->Change_Animation("ToolScoop_Repelled.anim");
	}

	else if ((CANT_DIG_AIR & Flag) != 0) {
		hr = Animator->Change_Animation("ToolScoop_Air.anim");
	}
	
	else {
		hr = Animator->Change_Animation("ToolScoop_Dig.anim");
		m_isDiggable = true;
	}

	return hr;
}

void CPlayerState_Dig::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();

	if (m_isTree) {
		if (Animator->isOverAnimTiming(0.5f)) {
			m_pPlayer->ActiveCollider_Tool(true,"Digged");
		}
	}
	if (m_isStone) {
		if (Animator->isOverAnimTiming(0.1f)) {
			m_pPlayer->ActiveCollider_Tool(true,"Digged");
		}
	}

	if (m_isTree || m_isDiggable) {
		if (Animator->isOverAnimTiming(0.2f) && !m_EffectComplete) {
			m_EffectComplete = true;
			Request_Effect();
		}
		if (Animator->isOverAnimTiming(0.5f)&& !m_DigComplete) {
			m_DigComplete = true;
			Make_Hole();
		}
	}

	if (m_isDigged) {
		Burry_Hole();
	}
}

HRESULT CPlayerState_Dig::OnExit()
{
	m_pPlayer->ActiveCollider_Tool(false, "");
	m_isTree = false;
	m_isDiggable = false;
	m_isDigged = false;
	m_DigComplete = false;
	m_isStone = false;
	m_EffectComplete = false;
	m_CloudComplete = false;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	
	return S_OK;
}

CState* CPlayerState_Dig::HandleTransition()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();


	if (Animator->isOverAnimTiming(0.95f)) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	return nullptr;
}

void CPlayerState_Dig::Render_State()
{
}

_uint CPlayerState_Dig::Get_InputMask() const
{
	return 0xffffffff;
}

void CPlayerState_Dig::Request_Effect()
{
	if (!m_EffectComplete)
		return;
	TILE_INDEX forward = m_pPlayer->Get_FowardIndex();
	_float4 pos = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(forward, ANCHOR::Center);
	pos.y += 0.5f;
	m_pPlayer->Request_Effect("Effect_GroundDump", { pos, pos });

}

void CPlayerState_Dig::Request_Cloud()
{/*
	if (m_CloudComplete)
		return;
	TILE_INDEX forward = m_pPlayer->Get_FowardIndex();
	_float4 pos = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(forward, ANCHOR::Center);
	pos.y += 0.5f;
	m_pPlayer->Request_Effect("Effect_Smoke", { pos, pos });*/
}

void CPlayerState_Dig::Make_Hole()
{
	if (!m_DigComplete)
		return;

	TILE_INDEX forward = m_pPlayer->Get_FowardIndex();
	_float4 pos = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(forward,ANCHOR::Center);

	CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_FieldHole" })
		.Position({ pos.x, pos.y, pos.z })
		.Build("Hole"); 

	CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pObj, { "GamePlay_Level","Field_Layer" });
	m_DigComplete = true;
}

void CPlayerState_Dig::Burry_Hole()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	if (Animator->isOverAnimTiming(0.5f)) {
		m_pPlayer->ActiveCollider_Tool(true, "BurryHole");
	}
	if (Animator->isOverAnimTiming(0.75f)) {
		//Request_Cloud();

	}
}

CPlayerState_Dig* CPlayerState_Dig::Create()
{
	return new CPlayerState_Dig;
}

void CPlayerState_Dig::Free()
{
	__super::Free();
}
