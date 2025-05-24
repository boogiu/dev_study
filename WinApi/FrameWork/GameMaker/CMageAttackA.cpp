#include "pch.h"
#include "CMageAttackA.h"
#include "CMonster.h"
#include "CObjMgr.h"
#include "CMageCore.h"
#include "CResourceMgr.h"

CMageAttackA::CMageAttackA() :m_bFired(false)
{
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 3,0 };
	aniFrame.isLoop = false;
	aniFrame.frameSpeed = 0.2f;
	aniFrame.m_spSize = { 100,100 };
}

CMageAttackA::~CMageAttackA()
{
}

void CMageAttackA::Enter(CMonster* monster)
{
	monster->SetAnim();
	m_pCore = nullptr;
	m_bFired = false;
	monster->Get_Anim()->Set_BreakPoint({ 2,0 });
}

void CMageAttackA::Update(CMonster* monster)
{
	if (!m_bFired&&monster->Get_Anim()->Calc_NowFrame({ 2,0 })) {
		m_pCore = static_cast<CMageCore*>(CObjMgr::Get_Instance()->ReQuestObject<CMageCore>(PROJECTILE));
		m_pCore->Initialize();
		m_pCore->Set_Pos(monster->Get_Info().fX, monster->Get_Collider()->Get_Info().LT().y);
		m_bFired = true;
		CResourceMgr::Get_Instance()->PlaySoundW(L"GoldenmaneWizard_Atk_Ready", 0.14f);
	}
	if (m_pCore&& (monster->Get_Target())) {
		if ((m_pCore)->Get_Anim()->Calc_NowFrame({ 3,7 })) {
			//발사
			INFO_EX target = monster->Get_Target()->Get_Info();
			INFO_EX Mine = monster->Get_Collider()->Get_Info();

			float dx = target.fX - Mine.fX;
			float dy = target.fY - Mine.LT().y;
			float angleRad = atan2(dy, dx); // 라디안 단위
			m_pCore->Set_Angle(angleRad);
			m_pCore->Fire();
			monster->Get_Anim()->Set_Stop(false);
		}
		if (monster->Get_Anim()->isAnimEnd()) {
			monster->ChangeState(MONSTER_IDLE);
		}
	}
}

bool CMageAttackA::Exit(CMonster* monster)
{
	if (!monster->Get_Anim()->isAnimEnd()) {
		if(m_pCore)
		m_pCore->Set_Active(false);
	}
	return false;
}
