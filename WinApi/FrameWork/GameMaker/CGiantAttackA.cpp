#include "pch.h"
#include "CGiantAttackA.h"
#include "CMonster.h"
#include "CAttackBox.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"
#include "CResourceMgr.h"

//ебе╛
CGiantAttackA::CGiantAttackA()
    :m_pAtkBox(nullptr), m_elapsedTime(0.f),m_iCheckDir(0),m_bDirCheck(false), m_pTackle_Effect(nullptr)
{
    aniFrame.startPoint = { 1,3 };
    aniFrame.endPoint = { 2,3 };
    aniFrame.isLoop = false;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 200,150 };
}

CGiantAttackA::~CGiantAttackA()
{
    if (m_pAtkBox) {
        Safe_Delete<CAttackBox*>(m_pAtkBox);
    }
}

void CGiantAttackA::Enter(CMonster* monster)
{
    monster->SetAnim();
    if (!m_pAtkBox) {
        m_pAtkBox = new CAttackBox(monster);
    }
    m_pAtkBox->Set_Target(PLAYER);
    m_elapsedTime = 0;
    m_bDirCheck = false;
    m_iCheckDir = 0;
    m_pTackle_Effect = nullptr;
    CResourceMgr::Get_Instance()->PlaySoundW(L"Recruit_MAA_Tackle_Ready", 0.14f);
}

void CGiantAttackA::Update(CMonster* monster)
{
    m_elapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
    if (m_elapsedTime > 1.7f) {

        if (!m_bDirCheck) {
            m_iCheckDir = monster->Get_Dir();
            m_bDirCheck = true;
        }
        monster->Set_DirStick(true);

        INFO atkInfo = {
            monster->Get_Collider()->Get_Info().fX + 120 * m_iCheckDir,
            monster->Get_Collider()->Get_Info().fY + 40,
            200,
            300
        };
        ANI_FRAME effectFrame;
        effectFrame.startPoint = { 0,0 };
        effectFrame.endPoint = { 0,3 };
        effectFrame.isLoop = false;
        effectFrame.m_spSize = { 150,100 };
        effectFrame.frameSpeed = 0.1f;
      
        if (!m_pTackle_Effect) {
            m_pTackle_Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
            m_pTackle_Effect->Initialize();
            m_pTackle_Effect->Set_Dir(m_iCheckDir);
            m_pTackle_Effect->PlayAnim(L"Tackle_Effect", effectFrame);
            CResourceMgr::Get_Instance()->PlaySoundW(L"Default_Dash_Tackle", 0.14f);
        }
       
        m_pTackle_Effect->Get_Info().Translate_Info(atkInfo);
        m_pTackle_Effect->Set_AutoDeaActive(true);

        m_pAtkBox->Set_Active();
        m_pAtkBox->Get_Info().Translate_Info(atkInfo);
        monster->Add_Velocity(8.5 * m_iCheckDir, 0.f);
    }
    if (m_elapsedTime > 3.3f) {
        if (m_pTackle_Effect) {
            //m_pTackle_Effect->Set_Active(false);
        }
        monster->ChangeState(MONSTER_IDLE);
    }
}

bool CGiantAttackA::Exit(CMonster* monster)
{
    monster->Set_DirStick(false);
   
    m_pAtkBox->Set_DeActive();
    return false;
}
