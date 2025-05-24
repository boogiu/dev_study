#include "pch.h"
#include "CBaseSkul.h"
#include "CResourceMgr.h"
#include "CPlayer.h"
#include "CTimeMgr.h"
#include "CProjectile.h"
#include "CSkulHead.h"
#include "CObjMgr.h"
#include "CAnim.h"
#include "CAttackBox.h"
#include "CEffectMgr.h"

CBaseSkul::CBaseSkul()
	:CoolTime(3.f), ElapsedTime(0.f), mySkul(nullptr), m_pAtkBox(nullptr), SwitchTime(0.f)
{

	m_skulType = normal;
	wsprintf(m_sName, L"Base_Skul");
	m_SkulSize = { 100,100 };
	bCanSkillB = false;

	Skul_Icon = L"Skul_ICON";
	SkillA_Icon = L"Base_Skill_icon";
	SkillB_Icon = L"Base_Skill2_icon";
}


CBaseSkul::~CBaseSkul()
{
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}


void CBaseSkul::Get_AnimFrame(ANI_FRAME& ani, const TCHAR* state)
{
	ani.m_spSize = { 100, 100 };
	ani.frameSpeed = 0.15f;
	ani.isLoop = true;

	if (state == L"IDLE") {
		ani.startPoint.x = 0;
		ani.startPoint.y = 0;
		ani.endPoint.x = 3;
		ani.endPoint.y = 0;
		ani.LoopPoint = { 0,0 };
	}

	if (state == L"MOVE") {
		ani.startPoint.x = 0;
		ani.startPoint.y = 1;
		ani.LoopPoint = { 0,1 };
		ani.endPoint.x = 7;
		ani.endPoint.y = 1;
	}
	if (state == L"DASH") {
		ani.startPoint.x = 0;
		ani.startPoint.y = 2;
		ani.isLoop = false;
		ani.endPoint.x = 0;
		ani.endPoint.y = 2;
	}
	if (state == L"JUMP") {
		ani.startPoint.x = 0;
		ani.startPoint.y = 3;
		ani.LoopPoint = { 0,3 };

		ani.endPoint.x = 1;
		ani.endPoint.y = 3;
	}
	if (state == L"FALL") {
		ani.isLoop = true;

		ani.startPoint = { 0,4 };
		ani.endPoint = { 4,4 };
		ani.LoopPoint = { 2,4 };
	}
	if (state == L"JUMPATK") {
		ani.isLoop = false;
		ani.startPoint = { 0,5 };
		ani.endPoint = { 3,5 };
	}
	if (state == L"ATTACKA") {
		ani.isLoop = false;
		ani.startPoint = { 0,6 };
		ani.endPoint = { 4,6 };
	}
	if (state == L"ATTACKB") {
		ani.isLoop = false;
		ani.startPoint = { 0,7 };
		ani.endPoint = { 3,7 };
	}
	if (state == L"SKILLA") {
		ani.isLoop = false;
		ani.startPoint = { 0,8 };
		ani.endPoint = { 2, 8 };
	}
	if (state == L"SKILLB") {
		ani.isLoop = false;
		ani.startPoint = { 0,0 };
		ani.endPoint = { 0,0 };
	}
	if (state == L"SWITCH") {
		m_bSwitchEnd = false;
		ani.frameSpeed = 0.05f;
		ani.isLoop = true;
		ani.startPoint = { 0,8 };
		ani.endPoint = { 6,8 };
		ani.LoopPoint = { 0,8 };
	}
}

void CBaseSkul::Update()
{
	if (!lstrcmp(m_sName, L"NoHead_Skul")) {
		ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
		if (ElapsedTime > CoolTime) {
			wsprintf(m_sName, L"Base_Skul");
			bCanSkillA = true;
			bCanSkillB = false;
			if(mySkul)
			mySkul->Set_Active(false);
			CBaseEffect* Effect1 = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
			Effect1->Initialize();
			ANI_FRAME effectFrame;
			effectFrame.startPoint = { 0,0 };
			effectFrame.endPoint = { 2,2 };
			effectFrame.isLoop = false;
			effectFrame.m_spSize = { 100,100 };
			effectFrame.frameSpeed = 0.02f;
			INFO info = { m_pOwner->Get_Info().fX,m_pOwner->Get_Info().fY, 100, 100 };
			Effect1->Get_Info().Translate_Info(info);
			Effect1->PlayAnim(L"Move_Skul_Effect", effectFrame);
			Effect1->Set_AutoDeaActive(true);
		}
	}
	else {
		ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
		if (ElapsedTime > CoolTime) {
			wsprintf(m_sName, L"Base_Skul");
			bCanSkillA = true;
			bCanSkillB = false;
		}
	}
}

void CBaseSkul::Late_Update()
{
}

void CBaseSkul::DoJumpAtk()
{
}

void CBaseSkul::CollisionSkillA(CObject* obj)
{
}

void CBaseSkul::CollisionSkillB(CObject* obj)
{
}

void CBaseSkul::EndSkillA()
{
}

void CBaseSkul::EndSkillB()
{
}

void CBaseSkul::Enter()
{
	SwitchTime = 0.f;
}

void CBaseSkul::Exit()
{
}

INFO CBaseSkul::Get_Skul_AtkPos()
{
	return { m_pOwner->Get_Info().fX + (m_pOwner->Get_Dir() * 40), m_pOwner->Get_Info().fY, 50.f, 100.f };
}

INFO CBaseSkul::Get_Skul_JUMPAtkPos()
{
	return { m_pOwner->Get_Info().fX + (m_pOwner->Get_Dir() * 40), m_pOwner->Get_Info().fY,  50.f, 100.f };
}

POINT CBaseSkul::Get_FRAME(CPlayer::STATE state)
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
		return { 2,6 };
		break;
	case CPlayer::ATTACKB:
		return { 1,7 };
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

void CBaseSkul::DoSkillA()
{
	CResourceMgr::Get_Instance()->PlaySoundW(L"Atk_Throw", 0.2f);
	mySkul = CObjMgr::Get_Instance()->ReQuestObject<CSkulHead>(OBJ_TAG::PROJECTILE);
	bCanSkillA = false;
	ElapsedTime = 0;
	bCanSkillB = true;
	mySkul->Initialize();
	mySkul->Set_Pos(m_pOwner->Get_Info().fX, m_pOwner->Get_Info().fY - 20);
	mySkul->Set_Dir(m_pOwner->Get_Dir());

	wsprintf(m_sName, L"NoHead_Skul");
}

void CBaseSkul::DoSkillB()
{
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 2,2 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 100,100 };
	effectFrame.frameSpeed = 0.02f;

	if (mySkul) {
		CBaseEffect* Effect1 = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
		Effect1->Initialize();
		INFO info = { m_pOwner->Get_Info().fX,m_pOwner->Get_Info().fY, 100, 100 };
		Effect1->Get_Info().Translate_Info(info);
		Effect1->PlayAnim(L"Move_Skul_Effect", effectFrame);
		Effect1->Set_AutoDeaActive(true);

		CResourceMgr::Get_Instance()->PlaySoundW(L"Skul_Reborn", 0.2f);
		m_pOwner->Set_Pos(mySkul->Get_Info().fX, mySkul->Get_Info().fY);
		bCanSkillB = false;
		mySkul->Set_Active(false);
		wsprintf(m_sName, L"Base_Skul");

		CBaseEffect* Effect2 = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
		Effect2->Initialize();
		INFO info2 = { m_pOwner->Get_Info().fX,m_pOwner->Get_Info().fY, 100, 100 };
		Effect2->Get_Info().Translate_Info(info2);
		Effect2->PlayAnim(L"Move_Skul_Effect", effectFrame);
		Effect2->Set_AutoDeaActive(true);
	}
}
//Move_Skul_Effect
void CBaseSkul::SwitchSkul()
{
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(m_pOwner);
	}
	SwitchTime += CTimeMgr::Get_Instance()->Get_Delta();
	m_pAtkBox->Set_Active();
	m_pAtkBox->Get_Info().Copy(m_pOwner->Get_Collider()->Get_Info());
	m_pAtkBox->Get_Info().Set_Size(200,100);
	m_pAtkBox->Set_Target(MONSTER);
	if (SwitchTime > 1.5f) {
		m_bSwitchEnd = true;
		SwitchTime = 0;
		m_pAtkBox->Set_DeActive();
	}
	m_pOwner->Add_Velocity(m_pOwner->Get_Dir(), 0.f);
}
