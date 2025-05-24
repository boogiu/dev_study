#include "pch.h"
#include "CMageMove.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CCollisionMgr.h"
#include "CResourceMgr.h"

CMageMove::CMageMove() :m_elapsedTime(0.f), m_bTeleporting(false), m_bFindTile(false)
{
	aniFrame.startPoint = { 3,2 };
	aniFrame.endPoint = { 3,4 };
	aniFrame.isLoop = false;
	aniFrame.frameSpeed = 0.1f;
	aniFrame.m_spSize = { 100,100 };
}

CMageMove::~CMageMove()
{
}

void CMageMove::Enter(CMonster* monster)
{
	m_elapsedTime = 0;
	monster->SetAnim();
	monster->Get_Anim()->Set_BreakPoint({ 4,3 });
	m_bFindTile = false;
	m_bTeleporting = false;
	m_bTeleportEnd = false;
	m_bMoveIn = false;
	m_bMoveOut = false;
}

void CMageMove::Update(CMonster* monster)
{
	if (monster->Get_Anim()->Calc_NowFrame({ 3,3 })) {
		m_bTeleporting = true; //집중 완료하면 이동
		if (!m_bMoveIn) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"Common_Teleport_In 1", 0.14f );
			m_bMoveIn = true;
		}
	}

	if (m_bTeleporting && !m_bTeleportEnd) {
		monster->Set_Render(false);
		monster->Set_Coll_Active(false);
		m_elapsedTime += CTimeMgr::Get_Instance()->Get_Delta();

		POINT movePt = CCollisionMgr::Get_Instance()->Check_SafetyPlatform(monster->Get_Rader(),
			monster->Get_Collider()->Get_Info().X_Size(), monster->Get_Collider()->Get_Info().Y_Size());

		if (m_elapsedTime > 0.5f) { //1초후 다시 애니메이션
			if (movePt.x == 0 && movePt.y == 0) {
				m_bFindTile = false;
			}
			else {
				m_bFindTile = true;
			}
			if (m_bFindTile) {
				monster->Set_Pos(movePt.x, movePt.y - monster->Get_Collider()->Get_Info().offsetBottom);
			}
			monster->Set_Render(true);
			monster->Set_Coll_Active(true);
			m_bTeleportEnd = true;
		}
	}
	if (m_bTeleportEnd) {
		if (!m_bMoveOut) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"Common_Teleport_Out_short", 0.14f);
			m_bMoveOut = true;
		}
		monster->Get_Anim()->Set_Stop(false);
		if (monster->Get_Anim()->isAnimEnd()) {
			if (monster->Get_Target()) {
				monster->ChangeState(MONSTER_ATTACKA);
			}
			else {
				monster->ChangeState(MONSTER_IDLE);
			}
		}
	}
}

bool CMageMove::Exit(CMonster* monster)
{
	return false;
}
