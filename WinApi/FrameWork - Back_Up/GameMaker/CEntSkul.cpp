#include "pch.h"
#include "CEntSkul.h"
#include "CResourceMgr.h"
#include "CAnim.h"
CEntSkul::CEntSkul()
{
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_Skul_RIGHT.bmp", L"Ent_Skul_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_Skul_LEFT.bmp", L"Ent_Skul_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_Skul_Skill_LEFT.bmp", L"Ent_Skul_SKILL_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_Skul_Skill_RIGHT.bmp", L"Ent_Skul_SKILL_R");

	wsprintf(m_sName, L"Ent_Skul");
	m_skulType = power;
	m_SkulSize = { 100,100 };
}

CEntSkul::~CEntSkul()
{
}

void CEntSkul::Update()
{
	
}

void CEntSkul::Late_Update()
{
	if ((m_pOwner->Get_Anim())->isAnimEnd()) {
		m_bSwitchEnd = true;
	}
}

void CEntSkul::Get_AnimFrame(ANI_FRAME& ani, const TCHAR* state)
{
	ani.m_spSize = { 100, 100 };
	ani.frameSpeed = 0.15f;
	ani.isLoop = true;
	m_pOwner->Get_Info().Set_Size(200.f, 200.f);

	wsprintf(m_sName, L"Ent_Skul");
	if (state == L"IDLE") {
		ani.startPoint = { 0,0 };
		ani.endPoint = { 3,0 };
		ani.LoopPoint = { 0,0 };
	}

	if (state == L"MOVE") {
		ani.startPoint = { 0,1 };
		ani.endPoint = { 7,1 };
		ani.LoopPoint = { 0,1 };
	}
	if (state == L"DASH") {
		ani.startPoint = { 0,2 };
		ani.endPoint = { 0,2 };
		ani.isLoop = false;
	}
	if (state == L"JUMP") {
		ani.startPoint = { 0,3 };
		ani.endPoint = { 0,3 };
		ani.LoopPoint = { 0,3 };
	}
	if (state == L"FALL") {
		ani.isLoop = true;
		ani.startPoint = { 0,4 };
		ani.endPoint = { 3,4 };
		ani.LoopPoint = { 2,4 };
	}
	if (state == L"JUMPATK") {
		ani.isLoop = false;
		ani.startPoint = { 0,5 };
		ani.endPoint = { 7,5 };
	}
	if (state == L"ATTACKA") {
		ani.isLoop = false;
		ani.startPoint = { 0,6 };
		ani.endPoint = { 6,6 };
	}
	if (state == L"ATTACKB") {
		ani.isLoop = false;
		ani.startPoint = { 0,7 };
		ani.endPoint = { 4,7 };
	}
	if (state == L"SWITCH") {
		ani.isLoop = false;
		ani.startPoint = { 0,8 };
		ani.endPoint = { 7,8 };
	}
	if (state == L"SKILLA") {
		wsprintf(m_sName, L"Ent_Skul_SKILL");
		m_pOwner->Get_Info().Add_OffsetY(100.f, 0.f);
		ani.m_spSize = { 300, 200 };
		ani.startPoint = { 0,0 };
		ani.endPoint = { 9, 0 };
		ani.isLoop = false;

	}
	if (state == L"SKILLB") {
		wsprintf(m_sName, L"Ent_Skul_SKILL");
		m_pOwner->Get_Info().Add_OffsetY(100.f, 0.f);


		ani.m_spSize = { 300, 200 };
		ani.isLoop = false;
		ani.startPoint = { 0,1};
		ani.endPoint = { 8, 1 };
	}
}

void CEntSkul::DoSkillA()
{
}

void CEntSkul::DoSkillB(){
}

void CEntSkul::SwitchSkul()
{
}

void CEntSkul::DoJumpAtk()
{
}
