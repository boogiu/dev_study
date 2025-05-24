#include "pch.h"
#include "CMageHit.h"
#include "CTimeMgr.h"
#include "CMonster.h"
#include "CResourceMgr.h"

CMageHit::CMageHit() : m_ElapsedTime(0.f)
{
    aniFrame.startPoint = { 1,1 };
    aniFrame.endPoint = { 1 ,2 };
    aniFrame.LoopPoint = { 2,1 };
    aniFrame.isLoop = false;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 100,100 };
}

CMageHit::~CMageHit()
{
}

void CMageHit::Enter(CMonster* monster)
{
    m_ElapsedTime = 0.f;
    monster->SetAnim();
    CResourceMgr::Get_Instance()->PlaySoundW(L"Whip_Hit", 0.14f);
}

void CMageHit::Update(CMonster* monster)
{
    m_ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
    monster->Add_Velocity(monster->Get_Dir() * -0.5f, 0.f);
    if (m_ElapsedTime > 0.25f) {
        monster->ChangeState(MONSTER_IDLE);
    }
}

bool CMageHit::Exit(CMonster* monster)
{
    return false;
}
