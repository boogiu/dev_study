#include "pch.h"
#include "CSoldierAttack.h"
#include "CMonster.h"
#include "CAttackBox.h"
#include "CResourceMgr.h"
CSoldierAttack::CSoldierAttack()
    :m_pAtkBox(nullptr),m_bAttack(false)
{
    aniFrame.startPoint = { 0,0 };
    aniFrame.endPoint = { 0,1 };
    aniFrame.isLoop = false;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 100,100 };
}

CSoldierAttack::~CSoldierAttack()
{
    Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CSoldierAttack::Enter(CMonster* monster)
{
    monster->Set_Velocity(0.f, 0.f);
    monster->SetAnim();
    if (!m_pAtkBox) {
        m_pAtkBox = new CAttackBox(monster);
    }
    m_pAtkBox->Set_Target(PLAYER);
    m_bAttack = false;
}

void CSoldierAttack::Update(CMonster* monster)
{
    if (monster->Get_Anim()->Calc_NowFrame({ 1,0 }) && monster->Get_State() == MONSTER_ATTACKA) {

        INFO atkPos = { monster->Get_Collider()->Get_Info().fX + (monster->Get_Dir() * 60), monster->Get_Collider()->Get_Info().fY,  50.f, 100.f };
        m_pAtkBox->Get_Info().Translate_Info(atkPos);
        m_pAtkBox->Set_Active();
        if (!m_bAttack) {
            CResourceMgr::Get_Instance()->PlaySoundW(L"Recruit_Atk_Ready01", 0.14f);
            CResourceMgr::Get_Instance()->PlaySoundW(L"Atk_Sword_Small 1", 0.14f);
            m_bAttack = true;
        }
    }

    if (monster->Get_Anim()->isAnimEnd()) {
        monster->ChangeState(MONSTER_IDLE);
        monster->ReleaseTarget();
    }
}

bool CSoldierAttack::Exit(CMonster* monster)
{
    m_pAtkBox->Set_DeActive();
    return false;
}
