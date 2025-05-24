#include "pch.h"
#include "CSoldierHit.h"
#include "CTimeMgr.h"
#include "CTimeMgr.h"
#include "CMonster.h"
#include "CResourceMgr.h"

CSoldierHit::CSoldierHit()
    :m_elapsedTime(0.f)
{
    aniFrame.startPoint = { 2,1 };
    aniFrame.endPoint = { 2,1 };
    aniFrame.isLoop = false;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 100,100 };
}

CSoldierHit::~CSoldierHit()
{
}

void CSoldierHit::Enter(CMonster* monster)
{
    m_elapsedTime = 0.f;
    monster->SetAnim();
    CResourceMgr::Get_Instance()->PlaySoundW(L"Whip_Hit", 0.14f);

}

void CSoldierHit::Update(CMonster* monster)
{
    m_elapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
    monster->Add_Velocity(monster->Get_Dir() * -8.f, 0.f);
    if (m_elapsedTime > 0.15f) {
        monster->ChangeState(MONSTER_IDLE);
    }
}

bool CSoldierHit::Exit(CMonster* monster)
{
    return false;
}
