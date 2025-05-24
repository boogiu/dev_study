#include "pch.h"
#include "CGiantMoveState.h"
#include "CMonster.h"
CGiantMoveState::CGiantMoveState()
    :m_bTackled(false)
{
    aniFrame.startPoint = { 3,3 };
    aniFrame.endPoint = { 3,5 };
    aniFrame.LoopPoint = { 3,3 };
    aniFrame.isLoop = true;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 200,150 };
}

CGiantMoveState::~CGiantMoveState()
{
}

void CGiantMoveState::Enter(CMonster* monster)
{
    monster->SetAnim();
}

void CGiantMoveState::Update(CMonster* monster)
{
    monster->Add_Velocity(1 * monster->Get_Dir(), 0.f);

    if (CObject* target = monster->Get_Target()) {
        float fDistance = fabs(target->Get_Info().fX - monster->Get_Info().fX);
        if (fDistance < 250&&monster->Can_Attack()) {
            if (m_bTackled) {
                monster->ChangeState(MONSTER_ATTACKB); // 내려찍기
                m_bTackled = false;
            }
            else {
                monster->ChangeState(MONSTER_ATTACKA); //태클
                m_bTackled = true;
            }
        }
    }
}

bool CGiantMoveState::Exit(CMonster* monster)
{
	return false;
}
