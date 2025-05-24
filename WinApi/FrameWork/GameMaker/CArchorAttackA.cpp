#include "pch.h"
#include "CArchorAttackA.h"
#include "CMonster.h"
#include "CObjMgr.h"
#include "CArrow.h"
#include "CEffectMgr.h"
#include "CResourceMgr.h"
CArchorAttackA::CArchorAttackA()
	:m_bFired(false), m_bSigned(false), m_atkSign(nullptr)
{
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 0,1 };
	aniFrame.isLoop = false;
	aniFrame.frameSpeed = 0.2f;
	aniFrame.m_spSize = { 100,100 };
}

CArchorAttackA::~CArchorAttackA()
{
}

void CArchorAttackA::Enter(CMonster* monster)
{
	CResourceMgr::Get_Instance()->PlaySoundW(L"Crossbow_Draw", 0.14f);

	m_bFired = false;
	m_bSigned = false;
	monster->SetAnim();
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 0,18 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 300,10 };
	effectFrame.frameSpeed = 0.02f;
	 m_atkSign = static_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	m_atkSign->Initialize();
	m_atkSign->Get_Info().fX = monster->Get_Collider()->Get_Info().fX - monster->Get_Dir() * 30;
	m_atkSign->Get_Info().fY = monster->Get_Collider()->Get_Info().fY + 40;
	
	CObject* target = monster->Get_Target();

	float distance = fabs(monster->Get_Info().fX - target->Get_Collider()->Get_Info().fX);
	
	m_atkSign->Get_Info().Set_Size(distance, 15);

	if (monster->Get_Dir() == 1) {
		m_atkSign->Get_Info().SetFromLeft(monster->Get_Info().fX);
	}
	else {
		m_atkSign->Get_Info().SetFromRight(monster->Get_Info().fX);
	}
	m_atkSign->PlayAnim(L"Arrow_Sign", effectFrame);
	m_atkSign->Set_AutoDeaActive(true);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Crossbow_Ready", 0.14f);
	m_bSigned = true;
}

void CArchorAttackA::Update(CMonster* monster)
{
	if (m_atkSign->Get_Anim()->isAnimEnd() && m_bSigned && !m_bFired) {
		CObject* arrow = CObjMgr::Get_Instance()->ReQuestObject<CArrow>(PROJECTILE);
		arrow->Set_Dir(monster->Get_Dir());
		arrow->Initialize();
		arrow->Set_Pos(monster->Get_Collider()->Get_Info().fX, monster->Get_Collider()->Get_Info().fY + 40);
		CResourceMgr::Get_Instance()->PlaySoundW(L"Crossbow_Fire", 0.14f);
		m_bFired = true;
	}
	if (monster->Get_Anim()->isAnimEnd()) {
		monster->ChangeState(MONSTER_IDLE);
	}
}

bool CArchorAttackA::Exit(CMonster* monster)
{
	monster->Attacked();
	m_atkSign == nullptr;
	return false;
}
