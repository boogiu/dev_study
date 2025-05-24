#include "pch.h"
#include "CPlayerJumpState.h"
#include "PlayerStates.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CSkul.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"
#include "CResourceMgr.h"

CPlayerJumpState::CPlayerJumpState()
	:JumpEffect(nullptr)
{
	jumpFrame.startPoint={ 0,0 };
	jumpFrame.endPoint={ 7,0 };
	jumpFrame.isLoop=false;
	jumpFrame.frameSpeed=0.15f;
	jumpFrame.m_spSize = { 150,100 };
	jumpFrame.LoopPoint = { 0,0 };
}

CPlayerJumpState::~CPlayerJumpState()
{
}
void CPlayerJumpState::Enter(CPlayer* player)
{
	if (player->isDoubleJump()) {
		JumpEffect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
		JumpEffect->Initialize();
		JumpEffect->Get_Info().fX = player->Get_Info().fX;
		JumpEffect->Get_Info().fY = player->Get_Info().fY;
		JumpEffect->Get_Info().Set_Size(150, 30);
		JumpEffect->PlayAnim(L"Player_Jump_Effect", jumpFrame);
	}
	player->Set_Velocity(0.f, -60.f);
	player->SetAnim(L"JUMP");
	player->Add_JumpCount(-1);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Default_Jump_Air", 0.14f);
}

void CPlayerJumpState::Update(CPlayer* player)
{
	player->Add_Velocity(0.f, GRAVITY );

	// 대시 입력
	if (CKeyMgr::Get_Instance()->GetKeyTap('Z')) {
		if (player->CanDash()) {
			player->ChangeState(CPlayer::DASH);
			return;
		}
	}

	// 2단 점프
	if (CKeyMgr::Get_Instance()->GetKeyTap('C') && player->CanJump()) {
		player->ChangeState(CPlayer::JUMP);
		return;
	}

	// 좌우 이동 (GetKeyPress 사용으로 지속 감지)
	if (CKeyMgr::Get_Instance()->GetKeyHold(VK_LEFT)) {
		player->Add_Velocity(-1.f, 0.f);
	}

	else if (CKeyMgr::Get_Instance()->GetKeyHold(VK_RIGHT)) {
		player->Add_Velocity(1.f, 0.f);
	}

	// 낙하로 전환
	if (player->Get_Velocity().fY >= 0.f) {
		player->ChangeState(CPlayer::FALL);
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('X')) {
		if (player->CanJumpAtk()) {
			player->ChangeState(CPlayer::JUMPATTACK);
			return;
		}
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('A')) {
		if (player->Get_Skul()->CanSkillA()) {
			player->ChangeState(CPlayer::SKILLA);
			return;
		}
	}

	if (CKeyMgr::Get_Instance()->GetKeyTap('S')) {
		if (player->Get_Skul()->CanSkillB()) {
			player->ChangeState(CPlayer::SKILLB);
			return;
		}
	}

	if (JumpEffect &&JumpEffect->Is_AnimEnd()) {
		JumpEffect->Set_Active(false);
	}
}


bool CPlayerJumpState::Exit(CPlayer* player)
{
	return true;
}

