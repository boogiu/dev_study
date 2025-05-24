#include "pch.h"
#include "CBossSlash.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"

CBossSlash::CBossSlash()
{
    aniFrame.frameSpeed = 0.1f;
    aniFrame.m_spSize = { 200, 200 };
    aniFrame.startPoint = { 0,0 };
    aniFrame.endPoint = { 4,0 };
    aniFrame.isLoop = false;
    aniFrame.LoopPoint = { 0, 0 };
}

CBossSlash::~CBossSlash()
{
    OutputDebugString(L"CBossSlash ¼Ò¸ê\n");
}

void CBossSlash::Enter(CBoss* boss)
{
    if (boss->Get_Target()->Get_Info().fX < boss->Get_Info().fX) {
        boss->Set_Dir(-1);
    }
    else {
        boss->Set_Dir(1);
    }
    boss->SetAnim(L"SLASH");
    m_slashed = false;
    m_elapsedTime = 0;
}

void CBossSlash::Update(CBoss* boss)
{
    if (boss->Get_Anim()->isAnimEnd()) {
        if (!m_slashed) {
            CObject* missile = CObjMgr::Get_Instance()->ReQuestObject<CSlashEffect>(PROJECTILE);
            missile->Set_Dir(boss->Get_Dir());
            float x= boss->Get_Target()->Get_Info().fX;
            float y= boss->Get_Target()->Get_Info().fY;
            missile->Set_Pos(x, y);
            missile->Initialize();
            m_slashed = true;
        }
        m_elapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
        if (m_elapsedTime > 1.f) {
            boss->ChangeState(BOSS_IDLE);
            m_elapsedTime = 0;
        }
    }
    
}

bool CBossSlash::Exit(CBoss* boss)
{
    return false;
}
