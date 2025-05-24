#include "pch.h"
#include "CBossHoming.h"
#include "CTimeMgr.h"
#include "CHomingPierce.h"
#include "CObjMgr.h"
#include "CResourceMgr.h"

CBossHoming::CBossHoming()
    : m_fTagetAngle(0.f), m_HomingCount(0), m_Targeting(false), m_elapsedTime(0.f), anim_out(false), AllFired(false)
{
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 300, 200 };
    aniFrame.isLoop = false;
    aniFrame.LoopPoint = { 0, 0 };
}

CBossHoming::~CBossHoming()
{
}

void CBossHoming::Enter(CBoss* boss)
{
    boss->Get_Info().Add_OffsetX(50.f, 50.f);
    aniFrame.startPoint = { 0, 0 };
    aniFrame.endPoint = { 1, 0 };
    m_fTagetAngle = 0.f;
    m_HomingCount = 0;
    m_Targeting = true;
    anim_out = false;
    AllFired = false;
    if (boss->Get_Target()->Get_Info().fX < boss->Get_Info().fX) {
        boss->Set_Dir(-1);
    }
    else {
        boss->Set_Dir(1);
    }
    boss->SetAnim(L"HOMING");
    INFO_EX info = boss->Get_Info();

    for (int i = 0; i < 3; ++i) {
        m_pierce[i] = static_cast<CHomingPierce*>(CObjMgr::Get_Instance()->ReQuestObject<CHomingPierce>(PROJECTILE));
        m_pierce[i]->Initialize();
        m_pierce[i]->Set_Dir(boss->Get_Dir());
        INFO_EX pierceInfo = m_pierce[i]->Get_Info();
        pierceInfo.fX = boss->Get_Info().fX + (10 * (i + 1) * boss->Get_Dir());
        pierceInfo.fY = boss->Get_Info().fY - (20 * (i + 1));
        m_pierce[i]->Get_Info().Copy(pierceInfo);
   
    }
}

void CBossHoming::Update(CBoss* boss)
{
    m_elapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
    CObject* target = boss->Get_Target();

    if (!target) return;

    INFO_EX targetPos = target->Get_Info();

    // 조준 각도 계산 (수정됨)
    if (m_HomingCount < 3) {
        float dx = targetPos.fX - m_pierce[m_HomingCount]->Get_Info().fX;
        float dy = targetPos.fY - m_pierce[m_HomingCount]->Get_Info().fY;
        m_pierce[m_HomingCount]->Set_Dir(boss->Get_Dir());
        float angleRadian = atan2f(dy, dx);
        m_pierce[m_HomingCount]->Set_Angle(angleRadian); // 내부에서 degree 변환 처리
    }

    if (m_Targeting) {
        for (int i = m_HomingCount; i < 3; ++i) {
            m_pierce[i]->Set_State(CHomingPierce::STATE::TARGETING);
        }

        if (m_elapsedTime > 1.f) {
            m_elapsedTime = 0;
            m_Targeting = false;
            aniFrame.startPoint = { 2, 0 };
            aniFrame.endPoint = { 5, 0 };
            aniFrame.isLoop = true;
            aniFrame.LoopPoint = { 2, 0 };
            boss->SetAnim(L"HOMING");
        }
        return;
    }

    if (!m_Targeting) {
        POINT FRAME = boss->Get_Anim()->Get_NowFrame();

        if (FRAME.x == 10) {
            boss->ChangeState(BOSS_IDLE);
        }

        if (FRAME.x == 5 && m_HomingCount < 3) {
            m_pierce[m_HomingCount]->Set_State(CHomingPierce::STATE::FIRE);
            aniFrame.startPoint = { 2, 0 };
            aniFrame.endPoint = { 5, 0 };
            aniFrame.isLoop = true;
            aniFrame.LoopPoint = { 2, 0 };
            boss->SetAnim(L"HOMING");
            m_HomingCount += 1;
            CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DimensionPierce_Sign", 0.2f);
            return;
        }

        if (m_HomingCount >= 3 && !anim_out) {
            aniFrame.startPoint = { 6, 0 };
            aniFrame.endPoint = { 10, 0 };
            aniFrame.isLoop = false;
            boss->SetAnim(L"HOMING");
            anim_out = true;
            AllFired = true;
        }
    }

    if (anim_out) {
        boss->ChangeState(BOSS_IDLE);
    }
}

bool CBossHoming::Exit(CBoss* boss)
{
    boss->Get_Info().Add_OffsetX(-50.f, -50.f);

    // 아직 모두 발사하지 않았으면 남은 투사체 전부 발사
    if (!AllFired)
    {
        for (int i = m_HomingCount; i < 3; ++i)
        {
            if (m_pierce[i])
            {
                m_pierce[i]->Set_State(CHomingPierce::STATE::FIRE);
            }
        }
    }

    return false;
}
