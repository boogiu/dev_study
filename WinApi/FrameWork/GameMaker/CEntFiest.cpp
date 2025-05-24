#include "pch.h"
#include "CEntFiest.h"
#include "CAnim.h"
#include "CAttackBox.h"
#include "CTimeMgr.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"
#include "CBoss.h"
#include "CResourceMgr.h"

CEntFiest::CEntFiest()
	:m_bFire(false), m_bSound(false), m_pAnim(nullptr)
{
}

CEntFiest::~CEntFiest()
{
	Release();
}

void CEntFiest::Initialize()
{
	fiestFrame.startPoint = { 0,0 };
	fiestFrame.endPoint = { 2,2};
	fiestFrame.isLoop = false;
	fiestFrame.frameSpeed = 0.03f;
	fiestFrame.m_spSize = { 50,50 };

	INFO info = { 50,50,150,150 };
	m_tInfo.Translate_Info(info);

	if (!m_pAnim) {
		m_pAnim = new CAnim(this);
	}
	m_pAnim->Set_Anim(L"EntSkul_Fiest_Effect", fiestFrame);
	m_pAnim->Set_BreakPoint({ 1,1 });
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(this);
	}
	m_pAtkBox->Set_Target(MONSTER);
	m_bFire = false;
	elapseTime = 0;
}

void CEntFiest::Update()
{


	if (m_pAnim) {
		m_pAnim->Update();

		if (!m_bFire&&m_pAnim->Calc_NowFrame({ 1,1 })) {
			m_bFire = true;
			m_pAtkBox->Set_Active();
			m_pAtkBox->Get_Info().Copy(m_tInfo);
			CCamera::Get_Instance()->Cam_Shake(0.4, true, 3);
		}
	}
	if (m_bFire) {
		elapseTime += CTimeMgr::Get_Instance()->Get_Delta();
	}
	if (elapseTime > 0.5f) {
		m_pAnim->Set_Stop(false);
		m_pAtkBox->Set_DeActive();
	}
}

void CEntFiest::Late_Update()
{
	if (m_pAnim) {
		m_pAnim->Late_Update();
		m_bActive = !m_pAnim->isAnimEnd();
	}
}

void CEntFiest::Render(HDC _hDC)
{
	if (m_pAnim)
	m_pAnim->Render(_hDC);
}

void CEntFiest::Release()
{
	Safe_Delete<CAnim*>(m_pAnim);
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CEntFiest::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (_hitType == ATTACK) {
		CResourceMgr::Get_Instance()->PlaySoundW(L"EntSkul_Root_Attack", 0.1f);

		if (!dynamic_cast<CBoss*>(object)) {
			object->Set_Velocity(0.f, 0.f);
			object->Add_Velocity(0.f, -50.f);
		}
	}
	CBaseEffect* Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	Effect->Initialize();
	INFO Info;
	if (collRECT.right - collRECT.left != 0) {
		Info = { (float(collRECT.right + collRECT.left) / 2),(float(collRECT.bottom + collRECT.top) / 2), 200, 200 };
	}
	else {
		Info = { m_tInfo.fX,m_tInfo.fY,200,200 };
	}
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 2,3 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 150,150 };
	effectFrame.frameSpeed = 0.05f;
	Effect->Get_Info().Translate_Info(Info);
	Effect->PlayAnim(L"Monster_Atk_Effect", effectFrame);
	Effect->Set_AutoDeaActive(true);
}
