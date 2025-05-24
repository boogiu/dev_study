#include "pch.h"
#include "CBossIdle.h"
#include "CTimeMgr.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"
#include "CResourceMgr.h"

CBossIdle::CBossIdle()
    :m_bWaitAnim(false)
{
    aniFrame.frameSpeed = 0.1f;
    aniFrame.m_spSize = { 200,200 };
    aniFrame.startPoint = { 0,0 };
    aniFrame.endPoint = { 8,0};
    aniFrame.LoopPoint = { 0,0 };
    aniFrame.isLoop = true;

}

CBossIdle::~CBossIdle()
{
}

void CBossIdle::Enter(CBoss* boss)
{
    
    if (boss->Get_Waiting()) {
        aniFrame.m_spSize = { 200,200 };
        aniFrame.startPoint = { 5,0 };
        aniFrame.endPoint = { 7,0 };
        aniFrame.isLoop = false;
        boss->SetAnim(L"WAIT");
        m_bWaitAnim = true;
        boss->Set_Velocity(0.f, -150);
    }
    else {
        aniFrame.m_spSize = { 200,200 };
        aniFrame.startPoint = { 0,0 };
        aniFrame.endPoint = { 8,0 };
        aniFrame.LoopPoint = { 0,0 };
        aniFrame.isLoop = true;
        boss->SetAnim(L"IDLE");
    }

    boss->Set_IDLE_END(false);
}

void CBossIdle::Update(CBoss* boss)
{
    if (boss->isOnGround()) {
        boss->Set_Velocity(0.f, 0.f);
        if (m_bWaitAnim) {
            aniFrame.m_spSize = { 200,200 };
            aniFrame.startPoint = { 0,0 };
            aniFrame.endPoint = { 8,0 };
            aniFrame.LoopPoint = { 0,0 };
            aniFrame.isLoop = true;
            boss->SetAnim(L"IDLE");
            m_bWaitAnim = false;
            CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Landing", 0.2f);
        }
        boss->Set_Waiting(false);
        boss->Set_IDLE_END(true);
        boss->Set_Coll_Active(true);
    }
    else {
        boss->Add_Velocity(0.f, GRAVITY);
    }
}

bool CBossIdle::Exit(CBoss* boss)
{
    return false;
}
