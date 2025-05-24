#include "pch.h"
#include "CSoldierMove.h"
#include "CMonster.h"

CSoldierMove::CSoldierMove()
{
    aniFrame.startPoint = { 3,2 };
    aniFrame.endPoint = { 2,4 };
    aniFrame.LoopPoint = { 3,2 };
    aniFrame.isLoop = true;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 100,100 };
}

CSoldierMove::~CSoldierMove()
{
}

void CSoldierMove::Enter(CMonster* monster)
{
    monster->SetAnim();
}

void CSoldierMove::Update(CMonster* monster)
{
    if (monster->Get_Target()) {
        INFO_EX target = monster->Get_Target()->Get_Info();
        INFO_EX monsterInfo = monster->Get_Info();
        float distance = fabs(target.fX - monsterInfo.fX);
        if (distance > 50) {
            monster->Add_Velocity(2 * monster->Get_Dir(), 0.f);
        }
    }
}

bool CSoldierMove::Exit(CMonster* monster)
{
    monster->Set_Velocity(0.f, 0.f);
    return false;
}
