#include "pch.h"
#include "CPlayerSkillAState.h"
#include "CPlayer.h"
#include "CSkul.h"
#include "CKeyMgr.h"

void CPlayerSkillAState::Enter(CPlayer* player)
{
	player->Get_Skul()->DoSkillA();
	player->SetAnim(L"SKILLA");
}

void CPlayerSkillAState::Update(CPlayer* player)
{
	player->Set_Velocity(0.f, 0.f);

	if (player->isAnimEnd()) {
		player->ChangeState(CPlayer::IDLE);
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('S')) {
		if (player->Get_Skul()->CanSkillB()) {
			player->ChangeState(CPlayer::SKILLB);
			return;
		}
	}
}

bool CPlayerSkillAState::Exit(CPlayer* player)
{
	return false;
}
