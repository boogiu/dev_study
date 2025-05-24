#include "pch.h"
#include "CArrow.h"
#include "CResourceMgr.h"
#include "CTimeMgr.h"
#include "CAttackBox.h"
#include "CResourceMgr.h"

CArrow::CArrow()
{
	m_fMoveSpeed = 50.f;
	m_tVector = { 15.f,10.f };
	INFO myInfo = { 0,0,200,40 };
	m_tInfo.Translate_Info(myInfo);
}

CArrow::~CArrow()
{
	Release();
}

void CArrow::Initialize()
{
	wstring path;

	if (m_Dir == 1) {
		path = L"Golden_Arrow_R";
	}
	else {
		path = L"Golden_Arrow_L";
	}
	memDC = CResourceMgr::Get_Instance()->Find_Image(path.c_str());

	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(this);
	}
	m_pAtkBox->Set_Target(PLAYER);
	m_pAtkBox->Get_Info().Copy(m_tInfo);
	m_pAtkBox->Set_Active();
}

void CArrow::Update()
{
	m_tInfo.fX += m_tVector.fX * m_fMoveSpeed * m_Dir * CTimeMgr::Get_Instance()->Get_Delta();
	if (m_pAtkBox)
		m_pAtkBox->Update();
}

void CArrow::Late_Update()
{
	if (m_pAtkBox)
		m_pAtkBox->Late_Update();
	m_pAtkBox->Get_Info().Copy(m_tInfo);

	m_tInfo.Update_INFO(m_tRect);
}

void CArrow::Render(HDC _hDC)
{
	m_tInfo.Update_INFO(m_tRect);
	if (CCamera::Get_Instance()->Out_of_Camera(m_tRect)) return;
	INFO_EX worldInfo = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	TransparentBlt(_hDC,
		worldInfo.LT().x,
		worldInfo.LT().y,
		worldInfo.X_Size(),
		worldInfo.Y_Size(),
		memDC,
		0, 0,
		118, 21,
		HOLLOW
	);
}

void CArrow::Release()
{
	if (m_pAtkBox) {
		Safe_Delete<CAttackBox*>(m_pAtkBox);
	}
}

void CArrow::Set_Active(bool _active)
{
	m_bActive = _active;
	if (_active == false) {
		m_pAtkBox->Set_DeActive();
	}
}

void CArrow::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (_hitType == ATTACK) {
		m_bActive = false;
		m_pAtkBox->Set_DeActive();
		CResourceMgr::Get_Instance()->PlaySoundW(L"arrow_hit", 0.14f);
	}
}
