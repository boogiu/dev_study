#include "pch.h"
#include "CEntSkul.h"
#include "CResourceMgr.h"
#include "CAnim.h"
#include "CAttackBox.h"
#include "CObjMgr.h"
#include "CEffectMgr.h"
#include "CBaseEffect.h"
#include "CPlayerRader.h"
#include "CEntFiest.h"
#include "CTimeMgr.h"

CEntSkul::CEntSkul()
	:m_pAtkBox(nullptr), m_pTarget(nullptr), GrabEffect(false), m_pRader(nullptr), NowFiest(0), MaxFiest(3),
	FiestEffect(false), FiestFired(false)
{
	wsprintf(m_sName, L"Ent_Skul");
	m_skulType = power;
	m_SkulSize = { 100,100 };
	Skul_Icon = L"Ent_ICON";
	SkillA_Icon = L"Ent_Skill_icon";
	SkillB_Icon = L"Ent_Skill2_icon";

	GrabFrame.startPoint = { 0,0 };
	GrabFrame.endPoint = { 2,3 };
	GrabFrame.isLoop = false;
	GrabFrame.m_spSize = { 550,550 };
	GrabFrame.frameSpeed = 0.02f;

	FiestSound = false;
}

CEntSkul::~CEntSkul()
{
	Safe_Delete<CAttackBox*>(m_pAtkBox);
	Safe_Delete<CPlayerRader*>(m_pRader);
}

void CEntSkul::Update()
{
	if (m_pOwner->GetState() == CPlayer::STATE::SKILLA) {
		if (m_pOwner->Get_Anim()->Calc_NowFrame({ 2,0 })) {
			if (m_pAtkBox) {
				m_pAtkBox->Set_Active();
			}
		}
		if (m_pOwner->Get_Anim()->Calc_NowFrame({ 3,0 })) {
			if (m_pTarget && !GrabEffect) {
				CBaseEffect* grab = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
				grab->Initialize();
				grab->Get_Info().Set_Size(150, 150);
				grab->Get_Info().fX = m_pTarget->Get_Collider()->Get_Info().fX;
				grab->Get_Info().fY = m_pOwner->Get_Collider()->Get_Info().fY;
				grab->PlayAnim(L"EntSkul_Grab_Effect", GrabFrame);

				grab->Set_AutoDeaActive(true);
				CResourceMgr::Get_Instance()->PlaySoundW(L"EntSkul_Grab", 0.1f);

				GrabEffect = true;
			}
		}
		if (m_pOwner->Get_Anim()->Calc_NowFrame({ 7,0 })) {
			if (m_pTarget) {
				m_pTarget->Set_VelocityX(0.f);
				m_pTarget->Add_Velocity(m_pOwner->Get_Dir() * -10, -5.f);
				GrabEffect = false;
			}
		}
	}
	if (m_pOwner->GetState() == CPlayer::STATE::SKILLB) {
		if (m_pOwner->Get_Anim()->Calc_NowFrame({ 5,1 })) {
			if (!FiestSound) {
				FiestSound = true;
				CResourceMgr::Get_Instance()->PlaySoundW(L"EntSkul_Root_Ready", 0.2f);
			}
			FireFiest();
		}
	}
	if (m_pOwner->GetState() == CPlayer::STATE::SWITCH) {
		if (m_pOwner->Get_Anim()->Calc_NowFrame({ 2,8 })) {
			SwtichSmoke();
		}
	}
	if (m_pRader) {
		m_pRader->Update();
	}
}

void CEntSkul::Late_Update()
{
	if ((m_pOwner->Get_Anim())->isAnimEnd()) {
		m_bSwitchEnd = true;
	}
	if (m_pRader) {
		m_pRader->Late_Update();
	}
}

void CEntSkul::Get_AnimFrame(ANI_FRAME& ani, const TCHAR* state)
{

	ani.m_spSize = { 100, 100 };
	ani.frameSpeed = 0.15f;
	ani.isLoop = true;

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
		ani.endPoint = { 6,5 };
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
		m_bSwitchEnd = false;
		ani.isLoop = false;
		ani.startPoint = { 0,8 };
		ani.endPoint = { 7,8 };
	}
	if (state == L"SKILLA") {
		wsprintf(m_sName, L"Ent_Skul_Skill");
		ani.m_spSize = { 300, 200 };
		ani.startPoint = { 0,0 };
		ani.endPoint = { 9, 0 };
		ani.isLoop = false;

	}
	if (state == L"SKILLB") {
		wsprintf(m_sName, L"Ent_Skul_Skill");
		ani.m_spSize = { 300, 200 };
		ani.isLoop = false;
		ani.startPoint = { 0,1 };
		ani.endPoint = { 7, 1 };
	}
}

void CEntSkul::DoSkillA()
{
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(m_pOwner);
	}
	INFO atkPos = { m_pOwner->Get_Info().fX + (m_pOwner->Get_Dir() * 80), m_pOwner->Get_Info().fY,  150.f, 200.f };
	m_pAtkBox->Get_Info().Translate_Info(atkPos);

	if (m_pOwner->Get_Dir() == 1) {
		m_pAtkBox->Get_Info().SetFromLeft(m_pOwner->Get_Info().fX);
	}
	else {
		m_pAtkBox->Get_Info().SetFromRight(m_pOwner->Get_Info().fX);
	}
	m_pAtkBox->Set_Target(MONSTER);
}

void CEntSkul::DoSkillB() {
	if (!m_pRader) {
		m_pRader = new CPlayerRader(m_pOwner);
	}
	m_pRader->Initialize();
	m_pRader->Get_Info().Copy(m_pOwner->Get_Collider()->Get_Info());
	m_pRader->Get_Info().Add_OffsetX(350.f, 350.f);
	m_pRader->Get_Info().Add_OffsetY(150.f, 50.f);
}

void CEntSkul::SwitchSkul()
{
}

void CEntSkul::DoJumpAtk()
{
	m_pOwner->Set_Velocity(0.f, -80.f);
}

void CEntSkul::CollisionSkillA(CObject* obj)
{
	m_pTarget = obj;
}

void CEntSkul::CollisionSkillB(CObject* obj)
{
}

void CEntSkul::EndSkillA()
{
	if (m_pAtkBox)
		m_pAtkBox->Set_DeActive();
	m_pTarget = nullptr;
}

void CEntSkul::EndSkillB()
{
	if (m_pAtkBox)
		m_pAtkBox->Set_DeActive();
	m_pTarget = nullptr;
	m_pRader->Set_Active(false);
	NowFiest = 0;
	fiestVector.clear();
	FiestSound = false;
}

void CEntSkul::FireFiest()
{
	for (CObject* target : fiestVector) {
		if (NowFiest < MaxFiest && NowFiest < fiestVector.size()) {
			CObject* fiest = CObjMgr::Get_Instance()->ReQuestObject<CEntFiest>(PROJECTILE);
			fiest->Initialize();
			// 위치 조정: 타겟의 아래쪽 + 20
			float fx = target->Get_Collider()->Get_Info().fX;
			float fy = target->Get_Collider()->Get_Info().RB().y+10;
			fiest->Set_Pos(fx,fy);
			fiest->Get_Info().SetFromBottom(fiest->Get_Info().fY);
			NowFiest += 1;
		}
	}
}

void CEntSkul::SwtichSmoke()
{
	if (m_bSwitchEnd) return;

	CBaseEffect* Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	Effect->Initialize();
	INFO Info = {m_pOwner->Get_Collider()->Get_Info().fX,m_pOwner->Get_Collider()->Get_Info().RB().y-40, 200,100};
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 2,2 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 100,50 };
	effectFrame.frameSpeed = 0.05f;
	Effect->Get_Info().Translate_Info(Info);
	Effect->PlayAnim(L"EntSkul_Swap_Effect", effectFrame);
	Effect->Set_AutoDeaActive(true);

	m_bSwitchEnd = true;
}

void CEntSkul::Enter()
{
	m_pOwner->SetAnim(L"IDLE");
}

void CEntSkul::Exit()
{
}

INFO CEntSkul::Get_Skul_AtkPos()
{
	return { m_pOwner->Get_Info().fX + (m_pOwner->Get_Dir() * 40), m_pOwner->Get_Info().fY, 50.f, 100.f };
}

INFO CEntSkul::Get_Skul_JUMPAtkPos()
{
	return { m_pOwner->Get_Collider()->Get_Info().fX , (float)(m_pOwner->Get_Collider()->Get_Info().RB().y),  100.f, 50.f };
}

POINT CEntSkul::Get_FRAME(CPlayer::STATE state)
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

void CEntSkul::Find_Target(CMonster* monster)
{
	auto iter = find_if(fiestVector.begin(), fiestVector.end(), [monster](CObject* data)->bool {return monster == data;});
	if (iter == fiestVector.end()) {
		fiestVector.push_back(monster);
	}
}

void CEntSkul::Not_Find_Target()
{
	m_pTarget = nullptr;
}
