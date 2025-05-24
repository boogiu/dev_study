#include "pch.h"
#include "CPlayerDashState.h"
#include "CPlayer.h"
#include "CTimeMgr.h"
#include "PlayerStates.h"
#include "CEffectMgr.h"
#include "CResourceMgr.h"

CPlayerDashState::CPlayerDashState()
    :m_DashSmoke(nullptr)
{
    m_fDashTime = 0.f;
    m_iDir = 1;
    Dash_Frame.startPoint = { 0,0 };
    Dash_Frame.endPoint = { 4,4 };
    Dash_Frame.isLoop = false;
    Dash_Frame.frameSpeed = 0.04f;
    Dash_Frame.m_spSize = { 150,150 };
}

CPlayerDashState::~CPlayerDashState()
{
}

void CPlayerDashState::Enter(CPlayer* player)
{
    player->SetAnim(L"DASH");
    m_fDashTime = 0.f;    
    m_fDashDuration = 0.2f;
    m_fDashSpeed = 10.f;
    m_iDir = player->Get_Dir();
    player->Add_DashCount(-1);
    player->Set_Coll_Active(false);
    if (!m_DashSmoke) {
        m_DashSmoke = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
        m_DashSmoke->Initialize();
        m_DashSmoke->Get_Info().Set_Size(150, 30);
  
        m_DashSmoke->Get_Info().fX = player->Get_Info().fX;
        m_DashSmoke->Get_Info().SetFromBottom(player->Get_Collider()->Get_Info().RB().y);
        m_DashSmoke->Set_Dir(m_iDir);
        m_DashSmoke->PlayAnim(L"Player_Dash_Smoke", Dash_Frame);
    }
    CResourceMgr::Get_Instance()->PlaySoundW(L"Default_Dash", 0.14f);

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
    player->Set_Coll_Active(true);
    m_DashSmoke->Set_Active(false); 
    m_DashSmoke = nullptr;
    return false;
}
