#include "pch.h"
#include "CCamera.h"
#include"CObject.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CFunction.h"
CCamera* CCamera::m_pInstance = nullptr;

CCamera::CCamera()
	:m_tPos{ }, m_szScreen{}, m_pTarget(nullptr), m_CamBorder{ WINCX,WINCY }, m_offsetX(0.f), m_offsetY(0.f)
	, m_fDuration(0.f), ElapsedTime(0.f), m_fShakeDelay(0.f), m_fShakeAmplitude(0.f), m_bVertical(false)
{

	tstRect = { 0,0, 500,100 };
}

CCamera::~CCamera()
{
}

void CCamera::Initialize()
{/*
	m_szScreen.cx = GetSystemMetrics(SM_CXSCREEN);
	m_szScreen.cy = GetSystemMetrics(SM_CYSCREEN);*/
	m_szScreen.cx = WINCX;
	m_szScreen.cy = WINCY;
	m_tPos = { m_szScreen.cx / 2,m_szScreen.cy / 2 };
	m_CinemaTime = 0;
	m_CinemaElapsed = 0;
	m_cinemaOffset = 0;
	m_bReadyToCinema = false;
}

void CCamera::Update()
{
	ShakingCam();
	//if (m_pTarget)
	//{
	//	const float speed = 4.f; // 카메라 따라가는 속도
	//	float delta = CTimeMgr::Get_Instance()->Get_Delta();
	//	float targetX = m_pTarget->Get_Info().fX;
	//	float targetY = m_pTarget->Get_Info().LT().y;

	//	m_tPos.x += static_cast<LONG>((targetX - m_tPos.x) * speed * 1.6f * delta);
	//	m_tPos.y += static_cast<LONG>((targetY - m_tPos.y) * speed * 2.5f * delta );
	//}
	//if ((m_tPos.x - m_szScreen.cx / 2) < 0) {
	//	m_tPos.x = m_szScreen.cx / 2;
	//}
	//if ((m_tPos.x + m_szScreen.cx / 2) > m_CamBorder.x) {
	//	m_tPos.x = m_szScreen.cx / 2;
	//}

	//if ((m_tPos.y - m_szScreen.cy / 2) < 0) {
	//	m_tPos.y = m_szScreen.cy / 2;
	//}
	//if ((m_tPos.y + m_szScreen.cy / 2) > m_CamBorder.y) {
	//	m_tPos.y = m_szScreen.cy / 2;
	//}
	if (m_pTarget)
	{
		const float speed = 4.f; // 카메라 따라가는 속도

		float delta = CTimeMgr::Get_Instance()->Get_Delta();
		float targetX = m_pTarget->Get_Info().fX;
		float targetY = m_pTarget->Get_Info().LT().y;

		// 부드럽게 따라가기
		m_tPos.x += static_cast<LONG>((targetX - m_tPos.x) * speed * delta);
		m_tPos.y += static_cast<LONG>((targetY - m_tPos.y) * speed * delta);
	}

	// 클램핑으로 맵 경계 제한
	m_tPos.x = Clamp(m_tPos.x, m_szScreen.cx / 2, m_CamBorder.x - m_szScreen.cx / 2);
	m_tPos.y = Clamp(m_tPos.y, m_szScreen.cy / 2, m_CamBorder.y - m_szScreen.cy / 2);

	Calcul_Diff();
	if (m_pTarget) {
		/*	swprintf_s(szBuffer,
				L"보고 있는 곳:%d,Y:%d \n 좌표 차이 : X:%d,Y:%d",
				m_tPos.x, m_tPos.y,
				m_tDiff.x,
				m_tDiff.y);*/
	}
}

void CCamera::Late_Update()
{
}

void CCamera::Render(HDC _hDC)
{
	Render_Cinematic(_hDC);

	if (m_pTarget)
	{
		//RECT tmp = { 210,10,450,100 };
		//DrawText(_hDC, szBuffer, lstrlen(szBuffer), &tmp, DT_CENTER);
		//// 중심 좌표 계산 (카메라 뷰의 중심)
		//int centerX = m_szScreen.cx / 2;
		//int centerY = m_szScreen.cy / 2;
		//
		//int lineLength = 10;

		//HPEN hOldPen = (HPEN)SelectObject(_hDC, CreatePen(PS_SOLID, 1, RGB(255, 0, 0)));
		//MoveToEx(_hDC, centerX - lineLength, centerY, NULL);
		//LineTo(_hDC, centerX + lineLength, centerY);
		//MoveToEx(_hDC, centerX, centerY - lineLength, NULL);
		//LineTo(_hDC, centerX, centerY + lineLength);
		//DeleteObject(SelectObject(_hDC, hOldPen));
	}
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

//INFO_EX CCamera::WorldToScreen(const INFO_EX& worldInfo) const
//{
//	INFO_EX newInfo = worldInfo;
//	newInfo.fX -= m_tDiff.x;
//	newInfo.fY -= m_tDiff.y;
//	return newInfo;
//}
//
//LINE_INFO CCamera::WorldToScreen(const LINE_INFO& worldLINE) const
//{
//	LINE_INFO line = worldLINE;
//	line.LT.x -= m_tDiff.x;
//	line.LT.y -= m_tDiff.y;
//	line.RB.x -= m_tDiff.x;
//	line.RB.y -= m_tDiff.y;
//	return line;
//}
INFO_EX CCamera::WorldToScreen(const INFO_EX& worldInfo) const
{
	INFO_EX newInfo = worldInfo;
	newInfo.fX -= m_tDiff.x + m_offsetX;  // 여기서만 흔들림 적용
	newInfo.fY -= m_tDiff.y + m_offsetY;
	return newInfo;
}
LINE_INFO CCamera::WorldToScreen(const LINE_INFO& worldLINE) const
{
	LINE_INFO line = worldLINE;
	line.LT.x -= m_tDiff.x + m_offsetX;
	line.LT.y -= m_tDiff.y + m_offsetY;
	line.RB.x -= m_tDiff.x + m_offsetX;
	line.RB.y -= m_tDiff.y + m_offsetY;
	return line;
}
bool CCamera::Out_of_Camera(INFO_EX& Info)
{
	RECT objRect;
	Info.Update_INFO(objRect); // 객체의 실제 위치 계산

	// 카메라 기준 화면 영역 계산
	float camLeft = m_tPos.x - m_szScreen.cx / 2.0f;
	float camRight = m_tPos.x + m_szScreen.cx / 2.0f;
	float camTop = m_tPos.y - m_szScreen.cy / 2.0f;
	float camBottom = m_tPos.y + m_szScreen.cy / 2.0f;

	// 완전히 화면 밖일 때 true
	if (objRect.right < camLeft || objRect.left > camRight ||
		objRect.bottom < camTop || objRect.top > camBottom)
	{
		return true;
	}

	return false; // 화면 안에 있음
}

bool CCamera::Out_of_Camera(RECT Info)
{
	// 카메라 기준 화면 영역 계산
	float camLeft = m_tPos.x - m_szScreen.cx / 2.0f;
	float camRight = m_tPos.x + m_szScreen.cx / 2.0f;
	float camTop = m_tPos.y - m_szScreen.cy / 2.0f;
	float camBottom = m_tPos.y + m_szScreen.cy / 2.0f;

	// 완전히 화면 밖일 때 true
	if (Info.right < camLeft || Info.left > camRight ||
		Info.bottom < camTop || Info.top > camBottom)
	{
		return true;
	}
	return false; // 화면 안에 있음
}

void CCamera::Camera_Reset()
{
}

void CCamera::Set_Border(int _x, int _y)
{
	m_CamBorder.x = _x;
	m_CamBorder.y = _y;
}
void CCamera::Cam_Shake(float _dur, bool Vertical, float amplitude)
{
	m_fDuration = _dur;
	m_bShake = true;
	m_bVertical = Vertical;
	m_fShakeAmplitude = amplitude;  // 흔들림 세기 저장
	m_fShakeDelay = 0.f;
	ElapsedTime = 0.f;
}

void CCamera::Cinematic(wstring word, float duration)
{
	m_CinemaStr = word;
	m_CinemaTime = duration;
}


void CCamera::Calcul_Diff()
{
	m_tDiff.x = m_tPos.x - (m_szScreen.cx / 2);
	m_tDiff.y = m_tPos.y - (m_szScreen.cy / 2);
}


void CCamera::ShakingCam()
{
	if (!m_bShake) return;

	ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
	m_fShakeDelay += CTimeMgr::Get_Instance()->Get_Delta();

	if (ElapsedTime > m_fDuration) {
		m_bShake = false;
		m_offsetY = 0.f;
		m_offsetX = 0.f;
		ElapsedTime = 0.f;
		m_fShakeDelay = 0.f;
	}
	else {
		if (m_fShakeDelay > 0.02f) {
			float strength = 1.f - (ElapsedTime / m_fDuration);  // 감쇠 효과
			float offset = CFunction::GetFloat(-m_fShakeAmplitude, m_fShakeAmplitude) * strength;

			if (m_bVertical)
				m_offsetY = offset;
			else
				m_offsetX = offset;

			m_fShakeDelay = 0.f;
		}
	}
}

void CCamera::Render_Cinematic(HDC _hDC)
{
	if (m_CinemaTime == 0) return;

	float DT = CTimeMgr::Get_Instance()->Get_Delta();
	m_CinemaElapsed += DT;
	float QuartTime = m_CinemaTime / 4;

	if (QuartTime > m_CinemaElapsed) {
		//내려가기
		m_cinemaOffset += DT * 90;
		m_bReadyToCinema = false;
	}
	else if (QuartTime * 3 > m_CinemaElapsed) {
		m_bReadyToCinema = true;
	}
	else {
		//올라오기
		m_cinemaOffset -= DT * 90;
		m_bReadyToCinema = false;
	}
	if (m_CinemaElapsed > m_CinemaTime) {
		m_CinemaTime = 0;
		m_CinemaElapsed = 0;
	}
	//// 내부를 비우기 위해 NULL_BRUSH 사용
	HBRUSH hNewBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hNewBrush);
	Rectangle(_hDC, 0, 0, WINCX, m_cinemaOffset);
	Rectangle(_hDC, 0, WINCY - m_cinemaOffset, WINCX, WINCY);
	SelectObject(_hDC, hOldBrush);
	DeleteObject(hNewBrush);

	COLORREF oldColor = GetTextColor(_hDC);
	int oldBkMode = GetBkMode(_hDC);

	HFONT hFont = CFunction::CreateFontW(L"NotoSans-Medium", 36, true);
	HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);

	SetTextColor(_hDC, RGB(255, 255, 255));
	SetBkMode(_hDC, TRANSPARENT);
	RECT rc = { WINCX - 500,WINCY - 100,WINCX - 50 ,WINCY };
	if (m_bReadyToCinema) {
		DrawText(_hDC, m_CinemaStr.c_str(),
			static_cast<int>(m_CinemaStr.length()),
			&rc,
			DT_RIGHT);
	}
	// 원래대로 복원
	SetTextColor(_hDC, oldColor);
	SetBkMode(_hDC, oldBkMode);
	SelectObject(_hDC, hOldFont);
	DeleteObject(hFont);
}
