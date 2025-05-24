#include "pch.h"
#include "CPlayerIdleState.h"
#include "PlayerStates.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CSkul.h"

CPlayerIdleState::CPlayerIdleState()
{
}

CPlayerIdleState::~CPlayerIdleState()
{
}

void CPlayerIdleState::Enter(CPlayer* player)
{
	player->SetAnim(L"IDLE");
	player->Set_Velocity(0.f, 0.f);
}

void CPlayerIdleState::Update(CPlayer* player)
{
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_LEFT) ||
		CKeyMgr::Get_Instance()->GetKeyDown(VK_RIGHT)) {
		player->ChangeState(CPlayer::MOVE);
		return;
	}

	if (CKeyMgr::Get_Instance()->GetKeyDown('C')) {
		if (player->CanJump()) {
			player->ChangeState(CPlayer::JUMP);
			return;
		}
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('Z')) {
		if (player->CanDash()) {
			player->ChangeState(CPlayer::DASH);
			return;
		}
	}

	if (CKeyMgr::Get_Instance()->GetKeyTap('X')) {
		player->ChangeState(CPlayer::ATTACKA);
		return;
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
	if (CKeyMgr::Get_Instance()->GetKeyTap(VK_SPACE)) {
		if (player->CanSwitch()) {
			player->ChangeState(CPlayer::SWITCH);
			return;
		}
	}
	if (!player->isOnGround()) {
		player->ChangeState(CPlayer::FALL);
		return;
	}
}

bool CPlayerIdleState::Exit(CPlayer* obj)
{
	return false;
}