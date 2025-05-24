#include "pch.h"
#include "CPlayerJumpState.h"
#include "PlayerStates.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CSkul.h"
#include "CTimeMgr.h"

CPlayerJumpState::CPlayerJumpState()
{
}

CPlayerJumpState::~CPlayerJumpState()
{
}
void CPlayerJumpState::Enter(CPlayer* player)
{
	player->Set_Velocity(0.f, -2.f);

	player->SetAnim(L"JUMP");
	//player->Add_Velocity(0.f, -5.f);
	player->Add_JumpCount(-1);
}
void CPlayerJumpState::Update(CPlayer* player)
{
	player->Add_Velocity(0.f, GRAVITY * CTimeMgr::Get_Instance()->Get_Delta());

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
}


bool CPlayerJumpState::Exit(CPlayer* player)
{
	return true;
}

