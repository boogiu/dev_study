#include "pch.h"
#include "CPlayerJumpAtkState.h"
#include "CPlayer.h"
#include "CSkul.h"

void CPlayerJumpAtkState::Enter(CPlayer* player)
{
	player->SetAnim(L"JUMPATK");
	player->Set_Velocity(0.f, 0.f);
	player->set_JumpAtk(false);

}

void CPlayerJumpAtkState::Update(CPlayer* player)
{
	if (player->Get_Skul()->GetType() == CSkul::normal) {
		if (player->isAnimEnd()) {
			player->ChangeState(CPlayer::FALL);
		}
	}
	else if (player->Get_Skul()->GetType() == CSkul::power) {
			player->Add_Velocity(0.f, 0.1f);
			if (player->isAnimEnd()) {
				player->Add_Velocity(0.f, 1.f);
			}
			if (player->isOnGround()) {
				player->Set_Velocity(0.f, 0.f);
				if (player->isAnimEnd()) {
					player->ChangeState(CPlayer::FALL);
				}
			}
	}
}

bool CPlayerJumpAtkState::Exit(CPlayer* player)
{
	return false;
}
