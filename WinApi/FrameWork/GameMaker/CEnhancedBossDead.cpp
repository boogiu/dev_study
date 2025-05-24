#include "pch.h"
#include "CEnhancedBossDead.h"
#include "CResourceMgr.h"
#include "CEffectMgr.h"

CEnhancedBossDead::CEnhancedBossDead():SoundID(0)
{
}

CEnhancedBossDead::~CEnhancedBossDead()
{
}

void CEnhancedBossDead::Enter(CBoss* boss)
{
    aniFrame.startPoint = { 0,0 };
    aniFrame.endPoint = { 4,6 };
    aniFrame.frameSpeed = 0.05f;
    aniFrame.m_spSize = { 200,200 };
    if (boss->Get_Target()->Get_Info().fX < boss->Get_Info().fX) {
        boss->Set_Dir(-1);
    }
    else {
        boss->Set_Dir(1);
    }
    boss->SetAnim(L"DEAD");
    CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Scream", 0.1f);
    CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Awaken_Dead", 0.1f);
    DeadEffect(boss);

}

void CEnhancedBossDead::Update(CBoss* boss)
{
    if (boss->Get_Anim()->isAnimEnd()) {
        boss->Set_Active(false);
    }
}

bool CEnhancedBossDead::Exit(CBoss* boss)
{
    CResourceMgr::Get_Instance()->StopSound(SoundID);
    CResourceMgr::Get_Instance()->StopSound(CH_BGM);
    return false;
}

void CEnhancedBossDead::DeadEffect(CBoss* boss)
{
    CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Awaken_Dead_Impact", 0.1f, &SoundID, true);

    CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
    m_pEffcet->Initialize();
    INFO groundInfo = { boss->Get_Info().fX,boss->Get_Collider()->Get_Info().RB().y, 300, 400};
    ANI_FRAME effectFrame;
    effectFrame.startPoint = { 0,0 };
    effectFrame.endPoint = { 6,7 };
    effectFrame.isLoop = false;
    effectFrame.m_spSize = { 600,800 };
    effectFrame.frameSpeed = 0.1f;

    m_pEffcet->Get_Info().Translate_Info(groundInfo);
    m_pEffcet->Get_Info().SetFromBottom(boss->Get_Collider()->Get_Info().RB().y+50);

    m_pEffcet->Set_Dir(boss->Get_Dir());
    m_pEffcet->PlayAnim(L"BossC_Dead_effect", effectFrame);
    m_pEffcet->Set_AutoDeaActive(true);
    m_pEffcet->Set_Layer(POST);
}
