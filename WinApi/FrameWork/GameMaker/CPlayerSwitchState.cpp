#include "pch.h"
#include "CPlayerSwitchState.h"
#include "Skuls.h"
#include "CResourceMgr.h"

void CPlayerSwitchState::Enter(CPlayer* player)
{
	player->SwapSkul();
	player->SetAnim(L"SWITCH");
	CResourceMgr::Get_Instance()->PlaySoundW(L"Default_Switch", 0.14f);
}

void CPlayerSwitchState::Update(CPlayer* player)
{
	CSkul* tmp = (player->Get_Skul());
	tmp->SwitchSkul();
	if (tmp->isSwitchOver()) {
		player->ChangeState(CPlayer::IDLE);
	}
}

bool CPlayerSwitchState::Exit(CPlayer* player)
{
	return false;
}
