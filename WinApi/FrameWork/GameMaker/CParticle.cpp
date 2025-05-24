#include "pch.h"
#include "CParticle.h"
#include "CResourceMgr.h"
#include "CFunction.h"
#include <gdiplus.h>
#include "CTimeMgr.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

CParticle::CParticle()
	:Parts(nullptr), m_fAngle(0.f), m_fAngleSpeed(0.f)
{

}

CParticle::~CParticle()
{
	Release();
}

void CParticle::Initialize()
{
	int x = CFunction::GetInt(-30, 30);
	int y = CFunction::GetInt(-30, -20);
	m_tVector = {(float)x,(float)y};
	if (!m_pCollider) {
		m_pCollider = new CCollider(this);
	}
	m_pCollider->SetActive(true);
	m_pCollider->Get_Info().Copy(m_tInfo);
	m_pCollider->Get_Info().Set_Size(30, 15);
}
void CParticle::Update()
{
	if (m_pCollider) {
		m_pCollider->Update();
		m_pCollider->Update_Center(m_tInfo.fX, m_tInfo.fY);
	}

	float dt = CTimeMgr::Get_Instance()->Get_Delta();

	// 위치 이동
	m_tInfo.fX += m_tVector.fX * dt * 40;
	m_tInfo.fY += m_tVector.fY * dt * 20;

	CObject* target = CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER);

	// 충돌 체크
	if (CCollisionMgr::Get_Instance()->Trigger_With_Player(this)) {
		// 데굴 회전 반응
		m_fAngleSpeed = 360.f; // 초당 360도 돌기 시작
		m_tVector.fX += target->Get_Dir() * 2.f;
		//m_tVector.fY -= 45.f;
	}

	// 회전 갱신
	m_fAngle += m_fAngleSpeed * dt;

	// 마찰 효과로 점점 느려짐
	m_fAngleSpeed *= 0.95f; 
}


void CParticle::Late_Update()
{
	if (!m_bGround) {
		Add_Velocity(0.f, GRAVITY);
	}
	else {
		m_tVector.fY = 0;
	}
	m_tVector.fX *= 0.95f;
}

void CParticle::Render(HDC _hDC)
{
	//NFO_EX screenInfo;
	/// 중심 기준 좌표 + 크기 정보 포함
	//screenInfo = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	//GdiTransparentBlt(
	//	_hDC,
	//	screenInfo.LT().x, screenInfo.LT().y,
	//	screenInfo.X_Size(), screenInfo.Y_Size(),
	//	memDC,
	//	0, 0,
	//	m_tSize.cx,
	//	m_tSize.cy,
	//	HOLLOW
	//;
	
	m_tInfo.Update_INFO(m_tRect);
	if (CCamera::Get_Instance()->Out_of_Camera(m_tRect)) return;

	Graphics graphics(_hDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	INFO_EX screenInfo = CCamera::Get_Instance()->WorldToScreen(m_tInfo);

	float centerX = screenInfo.fX;
	float centerY = screenInfo.fY;
	float width = screenInfo.X_Size();
	float height = screenInfo.Y_Size();

	graphics.TranslateTransform(centerX, centerY);
	graphics.RotateTransform(m_fAngle);
	graphics.TranslateTransform(-centerX, -centerY);

	if (Parts)
	{
		graphics.DrawImage(Parts,
			(INT)(centerX - width / 2),
			(INT)(centerY - height / 2),
			(INT)width,
			(INT)height);
	}

	graphics.ResetTransform();
}

void CParticle::Release()
{

	Safe_Delete<CCollider*>(m_pCollider);
}

SIZE CParticle::GetBitmapSizeFromDC(HDC hMemDC)
{
	SIZE size = { 0, 0 };
	HBITMAP hBmp = (HBITMAP)GetCurrentObject(hMemDC, OBJ_BITMAP);

	if (!hBmp) {
		OutputDebugString(L"[오류] DC에 비트맵이 Select되어 있지 않음\n");
		return size;
	}

	BITMAP bmp{};
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	size.cx = bmp.bmWidth;
	size.cy = bmp.bmHeight;
	return size;
}

void CParticle::Set_Name(wstring name, int i)
{
	m_szName = L"../Image/Particle/Part_" + name+to_wstring(i) + L".png";
	// 경로 확인
	//MessageBoxW(nullptr, m_szName.c_str(), L"이미지 경로", MB_OK);
	#undef new
		Parts = new Gdiplus::Image(m_szName.c_str());
	#define new DBG_NEW
	
		if (Parts->GetLastStatus() != Gdiplus::Ok)
		{
			OutputDebugString((L"[ERROR] 이미지 로딩 실패: " + m_szName + L"\n").c_str());
			delete Parts;
			Parts = nullptr;
			m_tInfo.Translate_Info({ 0, 0, 0.f, 0.f });
			return;
		}
	UINT width = Parts->GetWidth();
	UINT height = Parts->GetHeight();
	m_tInfo.Translate_Info({ 0, 0, (float)width, (float)height });
}
