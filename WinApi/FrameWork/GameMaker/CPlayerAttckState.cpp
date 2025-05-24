#include "pch.h"
#include "CPlayerAttckState.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CAttackBox.h"
#include "CAnim.h"
#include "CResourceMgr.h"
#include "CSkul.h"
CPlayerAttckState::CPlayerAttckState()
	:m_pAtkBox(nullptr), atkAnim(false)
{
}

CPlayerAttckState::~CPlayerAttckState()
{
		Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CPlayerAttckState::Enter(CPlayer* player)
{
	if (player->GetState() == CPlayer::ATTACKB) {
		player->SetAnim(L"ATTACKB");
		CResourceMgr::Get_Instance()->PlaySoundW(L"Skul_Atk 1", 0.14f);
	}
	else if(player->GetState() == CPlayer::ATTACKA) {
		player->SetAnim(L"ATTACKA");
		CResourceMgr::Get_Instance()->PlaySoundW(L"Skul_Atk 2", 0.14f);
	}

	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(player);
	}
	atkAnim = false;
}

void CPlayerAttckState::Update(CPlayer* player)
{
	if (player->Get_Anim()->Calc_NowFrame(player->Get_Skul()->Get_FRAME(CPlayer::ATTACKA))&& //{2,6}
		player->GetState() == CPlayer::ATTACKA) {
		if (!atkAnim) {
			INFO atkPos = { player->Get_Info().fX + (player->Get_Dir() * 40), player->Get_Info().fY,  50.f, 100.f };
			m_pAtkBox->Get_Info().Translate_Info(atkPos);
			m_pAtkBox->Set_Target(MONSTER);
			m_pAtkBox->Set_Active();
			atkAnim = true;

			if (player->Get_Skul()->GetType() == CSkul::power) {
				CCamera::Get_Instance()->Cam_Shake(0.3, true, 3);
			}
		}
	}
	else if (player->Get_Anim()->Calc_NowFrame(player->Get_Skul()->Get_FRAME(CPlayer::ATTACKB)) && //{ 1,7 }
		player->GetState() == CPlayer::ATTACKB) {
		if (!atkAnim) {
			//INFO atkPos = { player->Get_Info().fX + (player->Get_Dir() * 40), player->Get_Info().fY,  50.f, 100.f };
			INFO atkPos = player->Get_Skul()->Get_Skul_AtkPos();
			m_pAtkBox->Get_Info().Translate_Info(atkPos);
			m_pAtkBox->Set_Target(MONSTER);
			m_pAtkBox->Set_Active();
			atkAnim = true;
			if (player->Get_Skul()->GetType() == CSkul::power) {
				CCamera::Get_Instance()->Cam_Shake(0.3, true, 1);
			}
		}
	}
	if (player->isAnimEnd()) {
		if (CKeyMgr::Get_Instance()->GetKeyDown('X')) {
			if (player->GetState() == CPlayer::ATTACKB) {
				player->ChangeState(CPlayer::ATTACKA);
				return;
			}
			else if (player->GetState() == CPlayer::ATTACKA) {
				player->ChangeState(CPlayer::ATTACKB);
				return;
			}
		}
		player->ChangeState(CPlayer::IDLE);
	}
}

bool CPlayerAttckState::Exit(CPlayer* player)
{
	m_pAtkBox->Set_DeActive();
	return false;
}
