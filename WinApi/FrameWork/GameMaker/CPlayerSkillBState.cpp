#include "pch.h"
#include "CPlayerSkillBState.h"
#include "CPlayer.h"
#include "CSkul.h"

void CPlayerSkillBState::Enter(CPlayer* player)
{
    player->Get_Skul()->DoSkillB();
    player->SetAnim(L"SKILLB");
    if (player->Get_Skul()->GetType() == CSkul::power) {
        player->Get_Info().Add_OffsetX(50.f, 50.f);
    }
}

void CPlayerSkillBState::Update(CPlayer* player)
{
    player->Set_Velocity(0.f, 0.f);

    if (player->isAnimEnd()) {
        player->ChangeState(CPlayer::IDLE);
    }
}

bool CPlayerSkillBState::Exit(CPlayer* player)
{
    if (player->Get_Skul()->GetType() == CSkul::power) {
        player->Get_Info().Add_OffsetX(-50.f, -50.f);
    }
    player->Get_Skul()->EndSkillB();
    return false;
}
