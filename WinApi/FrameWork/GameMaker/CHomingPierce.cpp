#include "pch.h"
#include "CHomingPierce.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CAttackLine.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

CHomingPierce::CHomingPierce()
	: PierceAnim(nullptr), PierceImg(nullptr), m_fAngle(0.f),
	m_bFired(false), m_iFrame(0), m_fFrameTime(0.f), m_AtkLine(nullptr), m_bAppear(false)
{
#undef new
	PierceAnim = new Image(L"../Image/Projectile/Homing_APPEAR.png");
	PierceImg = new Image(L"../Image/Projectile/HomingProjectile.png");
	#define new DBG_NEW  // 혹은 프로젝트에서 정의한 new로 복

	INFO info{ 50,50,120,120, };
	m_tInfo.Translate_Info(info);
	//m_pCollider = new CCollider(m_tInfo);
}

CHomingPierce::~CHomingPierce()
{
	Release();
}

void CHomingPierce::Initialize()
{
	m_tVector = { 950.f, 0.f };
	m_fAngle = 0.f;
	m_bFired = false;
	m_eState = STATE::APPEAR;
	m_iFrame = 0;
	m_fFrameTime = 0.f;
	m_fMoveSpeed = 950.f;
	if (!m_AtkLine) {
		m_AtkLine = new CAttackLine(this);
		m_AtkLine->Set_Target(PLAYER);
	}
	m_AtkLine->Set_Active();
	m_bAppear = false;

}

void CHomingPierce::Update()
{
	if (!m_bAppear) {
		Appear();
		m_bAppear = true;
	}
	__super::Update_RECT();
	float delta = CTimeMgr::Get_Instance()->Get_Delta();
	// 라인 정보 갱신
	float length = (float)m_tInfo.X_Size(); // 가로 길이 == 공격 범위
	POINT center;
	if (m_Dir == 1) {
		 center = { (LONG)(m_tInfo.LT().x), (LONG)(m_tInfo.fY) };
	}
	else {
		 center = { (LONG)(m_tInfo.RB().x), (LONG)(m_tInfo.fY) };
	}
	LINE_INFO line;
	line.LT = center;
	line.Set_By_Angle(m_fAngle, length);

	m_AtkLine->Set_Line(line); // 정확한 위치와 방향을 반영
	if (m_eState == STATE::APPEAR)
	{
		m_fFrameTime += delta;

		if (m_fFrameTime >= m_fFrameDelay)
		{
			m_fFrameTime -= m_fFrameDelay;
			++m_iFrame;
			if (m_iFrame >= m_iMaxFrame)
			{
				m_iFrame = m_iMaxFrame - 1;
			}
		}

		// fire 전에는 회전만 적용됨
		return;
	}
	if (m_eState == STATE::TARGETING) {
		//m_fAngle += delta * 360.f * m_Dir;
	}
	if (m_eState == STATE::FIRE) {
		m_tInfo.fX += cosf(m_fAngle) * m_fMoveSpeed * delta;
		m_tInfo.fY += sinf(m_fAngle) * m_fMoveSpeed * delta;
	}
	if (m_tInfo.LT().x > WINCX ||
		m_tInfo.LT().y > WINCY ||
		m_tInfo.RB().x < 0 ||
		m_tInfo.RB().y < 0
		) {
		m_bActive = false;
		m_AtkLine->Set_DeActive();
	}

	if (m_pCollider) {
		m_pCollider->Update();
		m_pCollider->Update_Center(m_tInfo.fX, m_tInfo.fY);
	}
}

void CHomingPierce::Late_Update()
{
}

void CHomingPierce::Render(HDC _hDC)
{
	Graphics graphics(_hDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	// 중심 기준 회전 설정
	graphics.TranslateTransform(m_tInfo.fX, m_tInfo.fY);
	graphics.RotateTransform(m_fAngle * 180.f / PI);
	graphics.TranslateTransform(-m_tInfo.fX, -m_tInfo.fY);
	//if (m_Dir == -1)
	//{
	//	graphics.ScaleTransform(-1, 1);                  // 좌우 반전
	//	graphics.TranslateTransform(-m_tInfo.fX * 2, 0);    // 보정: 반전 시 위치 보정
	//}
	// 상태별 렌더링
	if (m_eState == STATE::APPEAR)
	{
		graphics.DrawImage(
			PierceAnim,
			(INT)(m_tInfo.LT().x), (INT)(m_tInfo.LT().y),
			m_iFrame * 80, 0,
			80, 80,
			UnitPixel);
	}
	else
	{
		graphics.DrawImage(
			PierceImg,
			(INT)(m_tInfo.LT().x), (INT)(m_tInfo.LT().y),
			(INT)m_tInfo.X_Size(),
			(INT)m_tInfo.Y_Size());
	}
	if (m_tInfo.LT().x >= WINCX ||
		m_tInfo.LT().y >= WINCY ||
		m_tInfo.RB().x <= 0 ||
		m_tInfo.RB().y <= 0
		) {
		graphics.RotateTransform(0);
	}	
}

void CHomingPierce::Release()
{
	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);

	if (m_AtkLine) {
		Safe_Delete<CAttackLine*>(m_AtkLine);
	}
}

void CHomingPierce::Appear()
{

	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
	m_pEffcet->Initialize();
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 0,4 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 80,80 };
	effectFrame.frameSpeed = 0.05f;

	m_pEffcet->Get_Info().Copy(m_tInfo);

	m_pEffcet->Set_Dir(m_Dir);
	m_pEffcet->PlayAnim(L"Homing_APPEAR", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	m_pEffcet->Set_Layer(POST);
}
