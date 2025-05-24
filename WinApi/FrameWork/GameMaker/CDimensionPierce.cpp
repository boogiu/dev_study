#include "pch.h"
#include "CDimensionPierce.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"
#include "CTimeMgr.h"
#include "CAnim.h"
#include "CAttackLine.h"
#include "CPlayer.h"
#include "CResourceMgr.h"

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

CDimensionPierce::CDimensionPierce()
	:m_Core(nullptr), PierceEnd(false), m_tTagetPos{}, m_fAngle(0.f), elapsedTime(0.f), m_AtkLine(nullptr)
{
#undef new
	PierceImg = new Image(L"../Image/Projectile/Dimension_Pierce.png");
#define new DBG_NEW
	m_tLaserFrame.startPoint = { 0,0 };
	m_tLaserFrame.endPoint = { 9,0 };
	m_tLaserFrame.isLoop = true;
	m_tLaserFrame.LoopPoint = { 0,0, };
	m_tLaserFrame.m_spSize = { 65,1500 };
	m_tLaserFrame.frameSpeed = 0.05f;
}

CDimensionPierce::~CDimensionPierce()
{
	Release();
}

void CDimensionPierce::Initialize()
{
	nowPoint = { 0,0 };
	PierceEnd = false;
	m_bFire = false;
	elapsedTime = 0;
	if (!m_AtkLine) {
		m_AtkLine = new CAttackLine(this);
	}
	m_AtkLine->Set_Target(PLAYER);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkDimensionPierce_Impact", 0.2f);
}

void CDimensionPierce::Update()
{

	// 중심 좌표
	float cx = m_tInfo.fX;
	float cy = m_tInfo.fY;

	LINE_INFO line;
	float halfLength = m_tLaserFrame.m_spSize.y / 2.f; // ← 이미지 전체 높이의 절반

	// 중심 기준에서 시작점(LT) 설정
	line.LT = {
		(LONG)(cx - cosf(m_fAngle) * halfLength),
		(LONG)(cy - sinf(m_fAngle) * halfLength)
	};

	// 방향 + 전체 길이로 RT 설정
	line.Set_By_Angle(m_fAngle, m_tLaserFrame.m_spSize.y);


	m_AtkLine->Set_Line(line); // 정확한 위치와 방향을 반영
	m_AtkLine->Set_Target(PLAYER);

	if (!m_bFire) return; // Fire 꺼지면 멈춤
	// 라인 정보 갱신

	elapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
	float delta = CTimeMgr::Get_Instance()->Get_Delta();

	if (elapsedTime > m_tLaserFrame.frameSpeed) {
		m_AtkLine->Set_Active();

		elapsedTime = 0.f;
		m_tInfo.Add_OffsetX(15.f, 15.f);

		int framePerRow = 10;
		int curIndex = AnimUtils::FrameToIndex(nowPoint, framePerRow);
		int endIndex = AnimUtils::FrameToIndex(m_tLaserFrame.endPoint, framePerRow);

		++curIndex;

		if (curIndex > endIndex) {
			nowPoint = m_tLaserFrame.isLoop ? m_tLaserFrame.LoopPoint : m_tLaserFrame.endPoint;
			m_bActive = false;
			m_AtkLine->Set_DeActive();
		}
		else {
			nowPoint = AnimUtils::IndexToFrame(curIndex, framePerRow);
		}
	}
}


void CDimensionPierce::Late_Update()
{
}

void CDimensionPierce::Render(HDC _hDC)
{
	Graphics graphics(_hDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	// 중심 기준 회전 설정
	graphics.TranslateTransform(m_tInfo.fX, m_tInfo.fY);
	graphics.RotateTransform(m_fAngle * 180.f / PI);
	graphics.TranslateTransform(-m_tInfo.fX, -m_tInfo.fY);

	graphics.DrawImage(
		PierceImg,
		(int)(m_tInfo.fX - m_tLaserFrame.m_spSize.x / 2),
		(int)(m_tInfo.fY - m_tLaserFrame.m_spSize.y / 2),
		nowPoint.x * m_tLaserFrame.m_spSize.x,
		nowPoint.y * m_tLaserFrame.m_spSize.y,
		m_tLaserFrame.m_spSize.x,
		m_tLaserFrame.m_spSize.y,
		UnitPixel);
	// 디버깅용 테스트 (직접 좌표 찍어보기)
		Rectangle(_hDC, m_AtkLine->Get_LineInfo().LT.x - 2, 
			m_AtkLine->Get_LineInfo().LT.y - 2, 
			m_AtkLine->Get_LineInfo().LT.x + 2, 
			m_AtkLine->Get_LineInfo().LT.y + 2);

		// DrawImage() 바로 아래에 추가
		//Pen pen(Color(255, 0, 255, 0), 2); // 초록색 테두리, 굵기 2
		//
		//graphics.DrawRectangle(
		//	&pen,
		//	(int)(m_tInfo.fX - m_tLaserFrame.m_spSize.x / 2),
		//	(int)(m_tInfo.fY - m_tLaserFrame.m_spSize.y / 2),
		//	m_tLaserFrame.m_spSize.x,            // 너비
		//	m_tLaserFrame.m_spSize.y             // 높이
		//);

	//Ellipse(_hDC, m_tInfo.fX - 5, m_tInfo.fY-5 ,m_tInfo.fX + 5, m_tInfo.fY + 5);
	graphics.ResetTransform(); 
}

void CDimensionPierce::Release()
{
	if (m_AtkLine) {
		Safe_Delete<CAttackLine*>(m_AtkLine);
	}
}

void CDimensionPierce::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	
}
