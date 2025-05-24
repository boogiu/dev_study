#include "pch.h"
#include "CMageCore.h"
#include "CAnim.h"
#include "CAttackBox.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"

CMageCore::CMageCore()
	:coreFrame{}, fAngle(0.f),m_pAnim(nullptr), m_bFire(false), m_soundID(0)
{
}

CMageCore::~CMageCore()
{
	Release();
}

void CMageCore::Initialize()
{
	coreFrame.startPoint = { 0,0 };
	coreFrame.endPoint = { 4,8 };
	coreFrame.LoopPoint = { 4,7 };
	coreFrame.isLoop = true;
	coreFrame.frameSpeed = 0.05f;
	coreFrame.m_spSize = {250,250};

	INFO info = { 50,50,350,350 };
	m_tInfo.Translate_Info(info);

	if (!m_pAnim) {
		m_pAnim = new CAnim(this);
	}
	m_pAnim->Set_Anim(L"Mage_Core", coreFrame);

	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(this);
	}
	m_pAtkBox->Set_Target(PLAYER);
	m_pAtkBox->Set_Active();
	m_soundID = 0;
	m_bFire = false;
	m_fMoveSpeed =80.f;
	CResourceMgr::Get_Instance()->PlaySoundW(L"GoldenmaneWizard_Projectile", 0.1f, &m_soundID,true);
	elapseTime = 0.f;
}

void CMageCore::Update()
{
	if (m_bFire) {
		m_tInfo.fX += cos(fAngle) * m_fMoveSpeed * CTimeMgr::Get_Instance()->Get_Delta();
		m_tInfo.fY += sin(fAngle) * m_fMoveSpeed * CTimeMgr::Get_Instance()->Get_Delta();
		m_pAtkBox->Get_Info().Copy(m_tInfo);
		m_pAtkBox->Get_Info().Set_Size(120.f, 120.f);
	}

	m_pAnim->Update();

	if (CCamera::Get_Instance()->Out_of_Camera(m_tInfo)) {
		//m_bActive = false;
		//m_pAtkBox->Set_DeActive();
		CResourceMgr::Get_Instance()->StopSound(m_soundID);
	}
	elapseTime += CTimeMgr::Get_Instance()->Get_Delta();
	if (elapseTime > 7.f) {
		CResourceMgr::Get_Instance()->StopSound(m_soundID);
	}
}

void CMageCore::Late_Update()
{
	m_pAnim->Late_Update();
}

void CMageCore::Render(HDC _hDC)
{
	m_pAnim->Render(_hDC);
}

void CMageCore::Release()
{
	Safe_Delete<CAnim*>(m_pAnim);
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}
void CMageCore::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (_hitType == HITTED) {
		
	}
	else {
		CResourceMgr::Get_Instance()->PlaySoundW(L"Hit_Flame_Short", 0.24f);
	}
}
