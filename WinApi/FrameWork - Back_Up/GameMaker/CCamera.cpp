#include "pch.h"
#include "CCamera.h"
#include"CObject.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
CCamera* CCamera::m_pInstance = nullptr;

CCamera::CCamera()
	:m_tPos{ }, m_szScreen{}, m_pTarget(nullptr)
{

	tstRect = { 0,0, 500,100 };
}

CCamera::~CCamera()
{
}

void CCamera::Initialize()
{
	m_szScreen.cx = GetSystemMetrics(SM_CXSCREEN);
	m_szScreen.cy = GetSystemMetrics(SM_CYSCREEN);
	m_tPos = { m_szScreen.cx / 2,m_szScreen.cy / 2 };

}

void CCamera::Update()
{
	if (m_pTarget)
	{
		const float speed = 4.f; // 카메라 따라가는 속도
		float delta = CTimeMgr::Get_Instance()->Get_Delta();

		/*	float targetX = m_pTarget->Get_Info().fX - (m_szScreen.cx / 2);
			float targetY = m_pTarget->Get_Info().fY - (m_szScreen.cy / 2);*/

		float targetX = m_pTarget->Get_Info().fX;
		float targetY = m_pTarget->Get_Info().fY;

		m_tPos.x += static_cast<LONG>((targetX - m_tPos.x) * speed * delta);
		m_tPos.y += static_cast<LONG>((targetY - m_tPos.y) * speed  * delta);
	}


	Calcul_Diff();

	if (m_pTarget) {
		swprintf_s(szBuffer,
			L"보고 있는 곳:%d,Y:%d \n 화면상 중앙 : X:%d,Y:%d",
			m_tPos.x, m_tPos.y,
			(m_szScreen.cx / 2),
			(m_szScreen.cy / 2));
	}
}

void CCamera::Late_Update()
{
}

void CCamera::Render(HDC _hDC)
{
	if (m_pTarget) {
		DrawText(_hDC, szBuffer, lstrlen(szBuffer), &tstRect, DT_CENTER);
	}



	HBRUSH hBr = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH orBr = (HBRUSH)SelectObject(_hDC, hBr);
	// 화면 중심을 빨간 점으로 찍는다면:
	int centerX = m_szScreen.cx / 2;
	int centerY = m_szScreen.cy / 2;

	Ellipse(_hDC,
		centerX - 5,
		centerY - 5,
		centerX + 5,
		centerY + 5);
	MoveToEx(_hDC, m_szScreen.cx / 2 - 10, m_szScreen.cy / 2, nullptr);
	LineTo(_hDC, m_szScreen.cx / 2 + 10, m_szScreen.cy / 2);
	MoveToEx(_hDC, m_szScreen.cx / 2, m_szScreen.cy / 2 - 10, nullptr);
	LineTo(_hDC, m_szScreen.cx / 2, m_szScreen.cy / 2 + 10);
	SelectObject(_hDC, orBr);
	DeleteObject(hBr);
}

void CCamera::Release()
{
}

void CCamera::Set_Target(CObject* _target)
{
	m_pTarget = _target;
}

INFO_EX CCamera::Get_RenderPos(INFO_EX Info)
{
	INFO_EX tmp(Info);
	tmp.Set_Pos(m_tDiff);
	return tmp;
}

INFO_EX CCamera::WorldToScreen(const INFO_EX& worldInfo) const
{
	INFO_EX newInfo = worldInfo;
	newInfo.fX -= m_tDiff.x;
	newInfo.fY -= m_tDiff.y;
	return newInfo;
}

void CCamera::Calcul_Diff()
{
	m_tDiff.x = m_tPos.x - (m_szScreen.cx / 2);
	m_tDiff.y = m_tPos.y - (m_szScreen.cy / 2);
}
