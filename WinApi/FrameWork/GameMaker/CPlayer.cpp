#include "pch.h"
#include "CPlayer.h"
#include "CResourceMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

CPlayer::CPlayer() : m_NowSTATE(IDLE), m_animIndex(0.f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_A.bmp", L"Player_IDLE");
	m_tInfo.fX = WINCX >> 1;
	m_tInfo.fY = WINCY >> 1;
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 200.f;

	//IDLE 0,0~3,0
	//RUN  0,1~ 7,1~0,2
	m_vecAnim.push_back({ {0,0},{1,0},{2,0},{3,0} });
	m_vecAnim.push_back({ {0,1},{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{0,2} });
	m_vecAnim.push_back({ {0,3},{1,3}});
	m_vecAnim.push_back({ {0,4},{1,4}});
	m_vecAnim.push_back({ {0,5},{1,5},{2,5} });
}

void CPlayer::Update()
{
	__super::Update_RECT();
	Key_Update();
	State_Check();

}

void CPlayer::Late_Update()
{
	m_animIndex += 0.008f;
	if (m_animIndex > m_vecAnim[m_NowSTATE].size()) {
		m_animIndex = 0;
	}

	m_tInfo.fX += m_tVector.fX * CTimeMgr::Get_Instance()->Get_Delta() * 30.f;
	m_tInfo.fY += m_tVector.fY * CTimeMgr::Get_Instance()->Get_Delta() * 100.f;

	m_tVector.fY += CTimeMgr::Get_Instance()->Get_Delta() * 1.1f;


	if (m_tCollRC.bottom >= 400) {
		m_tVector.fY = 0;
	}
}

void CPlayer::Render(HDC _hDC)
{
	Rectangle(_hDC, m_tRendRC.left, m_tRendRC.top, m_tRendRC.right, m_tRendRC.bottom);
	//Rectangle(_hDC, m_tCollRC.left, m_tCollRC.top, m_tCollRC.right, m_tCollRC.bottom);

	//__super::Render_BMP(L"Player_IDLE", _hDC, m_vecAnim[m_NowSTATE][m_animIndex]);
	__super::Render_BMP(L"Player_IDLE", _hDC, m_vecAnim[m_NowSTATE][m_animIndex]);

}

void CPlayer::Release()
{
	DeleteDC(memDC);
}

void CPlayer::Key_Update()
{
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_LEFT)) {
		m_tVector.fX = -1.f;
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_RIGHT)) {
		m_tVector.fX = 1.f;
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_SPACE)) {
		m_tVector.fY = -1.f;
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown('Z')) {
		m_tVector.fX += (1.3f * m_tVector.fX);
	}
	//friction
	m_tVector.fX *= 0.998f; 
	//m_tVector.fY *= 0.98f;
	
	if (fabsf(m_tVector.fX) < 0.001f) {
		m_tVector.fX = 0;
	}
	if (fabsf(m_tVector.fY) < 0.001f) {
		m_tVector.fY = 0;
	}
}

void CPlayer::State_Check()
{
	if (fabsf(m_tVector.fY) < 0.1f) {
		m_NowSTATE = LANDING;
	}
	else if (m_tVector.fY < 0) {
	}
	else if(m_tVector.fY > 0) {
			m_NowSTATE = FALLING;
	}
	if (m_tVector.fX != 0) {
		m_NowSTATE = RUN;
	}
	if (m_tVector.fX == 0) {
		m_NowSTATE = IDLE;
	}
}
