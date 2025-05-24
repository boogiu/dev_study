#include "pch.h"
#include "CPlayerJumpAtkState.h"
#include "CPlayer.h"
#include "CAttackBox.h"
#include "CSkul.h"
#include "CResourceMgr.h"
#include "CAnim.h"

CPlayerJumpAtkState::~CPlayerJumpAtkState()
{
		Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CPlayerJumpAtkState::Enter(CPlayer* player)
{
	player->Get_Skul()->DoJumpAtk();
	player->SetAnim(L"JUMPATK");
	player->Set_Velocity(0.f, 0.f);
	player->set_JumpAtk(false);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Skul_Jump_Atk", 0.14f);
	sound = false;
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(player);
	}

	//INFO atkPos = { player->Get_Info().fX + (player->Get_Dir() * 40), player->Get_Info().fY,  50.f, 100.f };
	
	m_pAtkBox->Set_Target(MONSTER);
	m_pAtkBox->Set_Active();
}

void CPlayerJumpAtkState::Update(CPlayer* player)
{
	INFO atkPos = player->Get_Skul()->Get_Skul_JUMPAtkPos();
	m_pAtkBox->Get_Info().Translate_Info(atkPos);

	if (player->Get_Skul()->GetType() == CSkul::normal) {
		if (player->isAnimEnd()) {
			player->ChangeState(CPlayer::FALL);
		}
	}
	else if (player->Get_Skul()->GetType() == CSkul::power) {
			if (player->Get_Anim()->Calc_NowFrame({3,5})) {
				player->Add_Velocity(0.f, 180.f);
			}

			if (player->isOnGround()) {
				CCamera::Get_Instance()->Cam_Shake(0.3, false, 4);
				if (!sound) {
					CResourceMgr::Get_Instance()->PlaySoundW(L"Atk_Dust", 0.1f);
					sound = true;
				}
				if (player->isAnimEnd()) {
					player->ChangeState(CPlayer::FALL);
				}
			}
	}
}

bool CPlayerJumpAtkState::Exit(CPlayer* player)
{
	m_pAtkBox->Set_DeActive();
	return false;
}
