#include "pch.h"
#include "CArchorHit.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"

CArchorHit::CArchorHit()
    :m_ElapsedTime(0.f)
{
    aniFrame.startPoint = { 2,2 };
    aniFrame.endPoint = { 2,2 };
    aniFrame.isLoop = false;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 100,100 };
}

CArchorHit::~CArchorHit()
{
}

void CArchorHit::Enter(CMonster* monster)
{
    m_ElapsedTime = 0.f;
    monster->SetAnim();
    CResourceMgr::Get_Instance()->PlaySoundW(L"Whip_Hit", 0.14f);

}

void CArchorHit::Update(CMonster* monster)
{
    m_ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
    monster->Add_Velocity(monster->Get_Dir() * -8.f, 0.f);
    if (m_ElapsedTime > 0.15f) {
        monster->ChangeState(MONSTER_IDLE);
    }
}

bool CArchorHit::Exit(CMonster* monster)
{
	return false;
}

