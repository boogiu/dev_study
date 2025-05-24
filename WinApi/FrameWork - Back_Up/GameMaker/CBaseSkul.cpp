#include "pch.h"
#include "CBaseSkul.h"
#include "CResourceMgr.h"
#include "CPlayer.h"
#include "CTimeMgr.h"
#include "CProjectile.h"
#include "CSkulHead.h"
#include "CObjMgr.h"
#include "CAnim.h"
CBaseSkul::CBaseSkul()
	:CoolTime(3.f), ElapsedTime(0.f), mySkul(nullptr)
{
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Base_Skul_RIGHT.bmp", L"Base_Skul_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Base_Skul_LEFT.bmp", L"Base_Skul_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/NoHead_Skul_RIGHT.bmp", L"NoHead_Skul_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/NoHead_Skul_LEFT.bmp", L"NoHead_Skul_L");

	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Skul_ICON.bmp", L"Skul_ICON");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Base_Skill_icon.bmp", L"Base_Skill_icon");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Base_Skill2_icon.bmp", L"Base_Skill2_icon");

	m_skulType = normal;
	wsprintf(m_sName, L"Base_Skul");
	m_SkulSize = { 100,100 };
	bCanSkillB = false;
}


CBaseSkul::~CBaseSkul()
{
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
			mySkul->Set_Active(false);
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

void CBaseSkul::DoSkillA()
{
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
	if (mySkul) {
		m_pOwner->Set_Pos(mySkul->Get_Info().fX, mySkul->Get_Info().fY);
		bCanSkillB = false;
		mySkul->Set_Active(false);
		wsprintf(m_sName, L"Base_Skul");
	}
}

void CBaseSkul::SwitchSkul()
{
		ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();

		if (ElapsedTime > 2.5f) {
			m_bSwitchEnd = true;
			ElapsedTime = 0;
		}
		m_pOwner->Add_Velocity(m_pOwner->Get_Dir(), 0.f);
}
