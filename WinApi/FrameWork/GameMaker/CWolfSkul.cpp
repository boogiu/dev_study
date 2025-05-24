#include "pch.h"
#include "CWolfSkul.h"
#include "CAnim.h"
#include "CAttackBox.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"
#include "CResourceMgr.h"

CWolfSkul::CWolfSkul()
	:m_pAtkBox(nullptr)
{
	wsprintf(m_sName, L"Wolf_Skul");

	m_skulType = normal;
	m_SkulSize = { 100,100 };
	Skul_Icon = L"Wolf_ICON";
	SkillA_Icon = L"Wolf_Skill_icon";
	SkillB_Icon = L"Wolf_Skill2_icon";

}

CWolfSkul::~CWolfSkul()
{
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CWolfSkul::Update()
{
}

void CWolfSkul::Late_Update()
{
}

void CWolfSkul::Get_AnimFrame(ANI_FRAME& ani, const TCHAR* state)
{
	ani.m_spSize = { 100, 100 };
	ani.frameSpeed = 0.1f;
	ani.isLoop = true;

	wsprintf(m_sName, L"Wolf_Skul");
	if (state == L"IDLE") {
		ani.startPoint = { 4,2 };
		ani.endPoint = { 2,3 };
		ani.LoopPoint = { 4,2 };
	}

	if (state == L"MOVE") {
		ani.startPoint = { 3,7 };
		ani.endPoint = { 0,8 };
		ani.LoopPoint = {3,7 };
	}
	if (state == L"DASH") {
		ani.startPoint = { 4,1 };
		ani.endPoint = { 6,1 };
		ani.isLoop = false;
	}
	if (state == L"JUMP") {
		ani.startPoint = { 3,3 };
		ani.endPoint = { 5,3 };
		ani.isLoop = false;
	}
	if (state == L"FALL") {
		ani.isLoop = true;
		ani.startPoint = { 7,1 };
		ani.endPoint = { 3,2 };
		ani.LoopPoint = { 1,2 };
	}
	if (state == L"JUMPATK") {
		ani.isLoop = false;
		ani.startPoint = { 6,3 };
		ani.endPoint = { 4,4 };
	}
	if (state == L"ATTACKA") {
		ani.isLoop = false;
		ani.startPoint = { 0,0 };
		ani.endPoint = { 6,0 };
	}
	if (state == L"ATTACKB") {
		ani.isLoop = false;
		ani.startPoint = { 7,0};
		ani.endPoint = { 3,1 };
	}
	if (state == L"SWITCH") {
		m_bSwitchEnd = false;
		ani.isLoop = false;
		ani.startPoint = { 3,6 };
		ani.endPoint = { 7,6 };
	}
	if (state == L"SKILLA") {
		ani.startPoint = { 5,4 };
		ani.endPoint = { 3,5 };
		ani.isLoop = false;

	}
	if (state == L"SKILLB") {
		ani.isLoop = false;
		ani.startPoint = { 4,5 };
		ani.endPoint = { 2,6 };
	}
}

void CWolfSkul::DoSkillA()
{
	CResourceMgr::Get_Instance()->PlaySoundW(L"Item_Whoosh", 0.2f);

	CBaseEffect* Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
	Effect->Initialize();
	Effect->Set_Dir(m_pOwner->Get_Dir());
	INFO Info = { m_pOwner->Get_Collider()->Get_Info().fX + m_pOwner->Get_Dir() * 40 ,m_pOwner->Get_Collider()->Get_Info().fY, 200,300 };
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 2,3 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 160,150 };
	effectFrame.frameSpeed = 0.05f;
	Effect->Get_Info().Translate_Info(Info);
	Effect->PlayAnim(L"Wolf_SkillA_Effect", effectFrame);
	Effect->Set_AutoDeaActive(true);

	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(m_pOwner);
	}
	m_pAtkBox->Get_Info().Translate_Info(Info);
	m_pAtkBox->Set_Target(MONSTER);
	m_pAtkBox->Set_Active();
}

void CWolfSkul::DoSkillB()
{
	CResourceMgr::Get_Instance()->PlaySoundW(L"Item_WindMail", 0.2f);

	CBaseEffect* Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
	Effect->Initialize();
	Effect->Set_Dir(m_pOwner->Get_Dir());
	INFO Info = { m_pOwner->Get_Collider()->Get_Info().fX + m_pOwner->Get_Dir() * 40 ,m_pOwner->Get_Collider()->Get_Info().fY, 
		200,300 };
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 1,3 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 160,150 };
	effectFrame.frameSpeed = 0.05f;
	Effect->Get_Info().Translate_Info(Info);
	Effect->PlayAnim(L"Wolf_SkillB_Effect", effectFrame);
	Effect->Set_AutoDeaActive(true);
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(m_pOwner);
	}
	m_pAtkBox->Get_Info().Translate_Info(Info);
	m_pAtkBox->Set_Target(MONSTER);
	m_pAtkBox->Set_Active();
}

void CWolfSkul::SwitchSkul()
{
	m_pOwner->Add_Velocity(4.f*m_pOwner->Get_Dir(), 0.f);
	if (m_pOwner->Get_Anim()->isAnimEnd()) {
		m_bSwitchEnd = true;
	}
}

void CWolfSkul::DoJumpAtk()
{
}

void CWolfSkul::CollisionSkillA(CObject* obj)
{
	CResourceMgr::Get_Instance()->PlaySoundW(L"Blast_Hit", 0.2f);

}

void CWolfSkul::CollisionSkillB(CObject* obj)
{
	CResourceMgr::Get_Instance()->PlaySoundW(L"Blast_Hit", 0.2f);

}

void CWolfSkul::EndSkillA()
{
	if (m_pAtkBox)
		m_pAtkBox->Set_DeActive();
}

void CWolfSkul::EndSkillB()
{
	if (m_pAtkBox)
		m_pAtkBox->Set_DeActive();
}

void CWolfSkul::Find_Target(CMonster* monster)
{
}

void CWolfSkul::Not_Find_Target()
{
}

void CWolfSkul::Enter()
{/*
	float bottom = m_pOwner->Get_Info().RB().y;
	m_pOwner->Get_Info().SetFromBottom(bottom);*/
	m_pOwner->SetAnim(L"IDLE");
	m_pOwner->Get_Info().fY -= 200;
	m_pOwner->Get_Collider()->Get_Info().Add_OffsetY(-30.f, 60.f);
	m_bSwitchEnd = false;
}

void CWolfSkul::Exit()
{
	m_pOwner->Get_Collider()->Get_Info().Copy(m_pOwner->Get_Info());
	m_pOwner->Get_Collider()->Set_OffsetX(25.f, 25.f);
	m_pOwner->Get_Collider()->Set_OffsetY(40.f, 30.f);
}

INFO CWolfSkul::Get_Skul_AtkPos()
{
	return { m_pOwner->Get_Info().fX + (m_pOwner->Get_Dir() * 40), m_pOwner->Get_Info().fY, 50.f, 100.f };
}

INFO CWolfSkul::Get_Skul_JUMPAtkPos()
{
	return { m_pOwner->Get_Info().fX , m_pOwner->Get_Info().fY+40,  100.f, 50.f };
}

POINT CWolfSkul::Get_FRAME(CPlayer::STATE state)
{

	switch (state)
	{
	case CPlayer::IDLE:
		break;
	case CPlayer::MOVE:
		break;
	case CPlayer::DASH:
		break;
	case CPlayer::JUMP:
		break;
	case CPlayer::FALL:
		break;
	case CPlayer::ATTACKA:
		return{ 2,0 };
		break;
	case CPlayer::ATTACKB:
		return{ 0,1 };
		break;
	case CPlayer::JUMPATTACK:
		break;
	case CPlayer::SKILLA:
		break;
	case CPlayer::SKILLB:
		break;
	case CPlayer::SWITCH:
		break;
	case CPlayer::NONE:
		break;
	default:
		break;
	}
	return POINT();
}
