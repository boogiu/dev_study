#include "pch.h"
#include "CMeteorBullet.h"
#include "CAnim.h"
#include "CAttackBox.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"

CMeteorBullet::CMeteorBullet()
	:aniFrame{}, m_pAnim(nullptr)
{
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 3,2 };
	aniFrame.m_spSize={ 300,300 };
	INFO info = { 50,50,300,400 };
	m_tInfo.Translate_Info(info);
	m_tVector.fX = 800.f;
}

CMeteorBullet::~CMeteorBullet()
{
	Release();
}

void CMeteorBullet::Initialize()
{
	aniFrame.isLoop = true;
	aniFrame.m_spSize = { 300,300 };
	aniFrame.frameSpeed = 0.1f;
	if (!m_pAnim) {
		m_pAnim = new CAnim(this);
	}
	wstring path;
	if (m_Dir == -1) {
		path = L"Meteor_Bullet_L";
	}
	else {
		path = L"Meteor_Bullet_R";
	}
	m_pAnim->Set_Anim(path.c_str(), aniFrame);

	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(this);
	}
	m_pAtkBox->Set_Target(PLAYER);
	m_pAtkBox->Set_Active();
	m_pAtkBox->Get_Info().Copy(m_tInfo);
	m_pAtkBox->Get_Info().Add_OffsetY(-120.f,0.f);

	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkMeteor", 0.2f);
}

void CMeteorBullet::Update()
{
	m_tInfo.fX += m_tVector.fX * m_Dir * CTimeMgr::Get_Instance()->Get_Delta();
	m_pAnim->Update();
	m_pAtkBox->Get_Info().fX = m_tInfo.fX;
	m_pAtkBox->Get_Info().fY = m_tInfo.fY;
}

void CMeteorBullet::Late_Update()
{
	m_pAnim->Late_Update();

}

void CMeteorBullet::Render(HDC _hDC)
{
	m_pAnim->Render(_hDC);
}

void CMeteorBullet::Release()
{
	Safe_Delete<CAnim*>(m_pAnim);
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}
