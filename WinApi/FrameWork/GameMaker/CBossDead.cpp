#include "pch.h"
#include "CBossDead.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"

CBossDead::CBossDead()
    :elapsedTime(0.f)
{
}

CBossDead::~CBossDead()
{
}

void CBossDead::Enter(CBoss* boss)
{
    aniFrame.startPoint = { 0,0 };
    aniFrame.endPoint = { 5,0 };
    aniFrame.frameSpeed = 0.15f;
    aniFrame.m_spSize = { 200,200 };
    aniFrame.isLoop = false;
    elapsedTime = 0;
    if (boss->Get_Target()->Get_Info().fX < boss->Get_Info().fX) {
        boss->Set_Dir(-1);
    }
    else {
        boss->Set_Dir(1);
    }
    boss->SetAnim(L"DEAD");
    CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Sign", 0.4f);
}

void CBossDead::Update(CBoss* boss)
{
    if (boss->Get_Anim()->isAnimEnd()) {
        elapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
        if (elapsedTime > 2.4f) {
            boss->Set_Active(false);
        }
    }
}

bool CBossDead::Exit(CBoss* boss)
{
    return false;
}
