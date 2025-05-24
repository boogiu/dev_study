#include "pch.h"
#include "CSlashEffect.h"
#include "CAnim.h"
#include "CAttackLine.h"
#include "CResourceMgr.h"

CSlashEffect::CSlashEffect()
	:aniFrame{},m_pAnim(nullptr), m_AtkLine(nullptr)
{
	INFO info = { 0,0,300,300 };
	m_tInfo.Translate_Info(info);
}

CSlashEffect::~CSlashEffect()
{
	Release();
}

void CSlashEffect::Initialize()
{
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 2,2 };
	aniFrame.isLoop = false;
	aniFrame.m_spSize = { 300,300 };
	aniFrame.frameSpeed = 0.1f;
	if (!m_pAnim) {
		m_pAnim = new CAnim(this);
	}
	if (!m_AtkLine) {
		m_AtkLine = new CAttackLine(this);
		m_AtkLine->Set_Target(PLAYER);
	}
	wstring path;
	if (m_Dir == -1) {
		path = L"Slash_EFFECT_L";
		m_AtkLine->Get_LineInfo().LT = { m_tInfo.RB().x,m_tInfo.LT().y };
		m_AtkLine->Get_LineInfo().RB = { m_tInfo.LT().x,m_tInfo.RB().y };
	}
	else {
		path = L"Slash_EFFECT_R";
		m_AtkLine->Get_LineInfo().LT = m_tInfo.LT();
		m_AtkLine->Get_LineInfo().RB = m_tInfo.RB();
	}
	m_pAnim->Set_Anim(path.c_str(), aniFrame);
	m_AtkLine->Set_Active();
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Pierce_Impact", 0.4f);
}

void CSlashEffect::Update()
{
	m_pAnim->Update();
}

void CSlashEffect::Late_Update()
{
	m_pAnim->Late_Update();
	if (m_pAnim->isAnimEnd()) {
		m_bActive = false;
		m_AtkLine->Set_DeActive();
	}
}

void CSlashEffect::Render(HDC _hDC)
{
	m_pAnim->Render(_hDC);
}

void CSlashEffect::Release()
{
	Safe_Delete<CAnim*>(m_pAnim);
	Safe_Delete<CAttackLine*>(m_AtkLine);
}
