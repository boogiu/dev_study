#include "pch.h"
#include "CPlayerDashState.h"
#include "CPlayer.h"
#include "CTimeMgr.h"
#include "PlayerStates.h"

CPlayerDashState::CPlayerDashState()
{
    m_fDashTime = 0.f;
    m_iDir = 1;
}

CPlayerDashState::~CPlayerDashState()
{
}

void CPlayerDashState::Enter(CPlayer* player)
{
    player->SetAnim(L"DASH");
    m_fDashTime = 0.f;    
    m_fDashDuration = 0.2f;
    m_fDashSpeed = 8.f;
    m_iDir = player->Get_Dir();
    player->Add_DashCount(-1);
}

void CPlayerDashState::Update(CPlayer* player)
{
    m_fDashTime += CTimeMgr::Get_Instance()->Get_Delta();
    player->Set_Velocity(m_iDir * m_fDashSpeed, 0);
    if (m_fDashTime >= m_fDashDuration) {
        player->Set_Velocity(0.f, player->Get_Velocity().fY);

        if (player->isOnGround())
            player->ChangeState(CPlayer::IDLE);
        
        else
            player->ChangeState(CPlayer::FALL);
    }
}

bool CPlayerDashState::Exit(CPlayer* player)
{
    return false;
}
