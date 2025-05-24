#include "pch.h"
#include "CBaseEffect.h"
#include "CAnim.h"
#include "CAttackBox.h"

CBaseEffect::CBaseEffect()
	:m_pAnim(nullptr), m_tFrame{},m_bAnimEnd(false),m_eTag(HAS_DIRECTION), m_bAutoDective(false), m_SyncBox(nullptr)
	, m_eLayer(AFTER)
{
}

CBaseEffect::~CBaseEffect()
{
	Release();
}

void CBaseEffect::Initialize()
{
	if (!m_pAnim) {
		m_pAnim = new CAnim(this);
	}
	m_eLayer = AFTER;
}

void CBaseEffect::Update()
{
	if (m_pAnim) {
		m_pAnim->Update();
		m_bAnimEnd = m_pAnim->isAnimEnd();
		m_tInfo.Update_INFO(m_tRect);
	}
	if (m_bAutoDective) {
		m_bActive = !m_bAnimEnd;
		if (m_SyncBox&&!m_bActive) {
			m_SyncBox->Set_DeActive();
			m_SyncBox = nullptr;
		}
	}
}

void CBaseEffect::Late_Update()
{

	if (m_pAnim) {
		m_pAnim->Late_Update();
	}
}

void CBaseEffect::Render(HDC _hDC)
{
	if (m_pAnim) {
		m_pAnim->Render(_hDC);
	}
}

void CBaseEffect::Release()
{
	if (m_pAnim) {
		Safe_Delete<CAnim*>(m_pAnim);
	}

}

void CBaseEffect::PlayAnim(wstring pFrameKey, ANI_FRAME _Frame)
{
	m_tFrame = _Frame;
	wstring tmpKey = pFrameKey;
	if (m_eTag == HAS_DIRECTION) {
		if (m_Dir ==1) {
			tmpKey += L"_R";
		}
		else {
			tmpKey += L"_L";
		}
	}
	Set_Name(tmpKey);
	Set_Anim(_Frame);
}

void CBaseEffect::Set_Anim(ANI_FRAME _Frame)
{
	if (m_pAnim) {
		m_pAnim->Set_Anim(m_EffectName.c_str(), _Frame);
	}
}

void CBaseEffect::Set_Name(wstring name)
{
	m_EffectName = name;
}
