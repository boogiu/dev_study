#include "pch.h"
#include "CDmgText.h"
#include "CTimeMgr.h"
#include "CFunction.h"

CDmgText::CDmgText()
{
}

CDmgText::~CDmgText()
{
}

void CDmgText::Initialize()
{
	m_tInfo.Translate_Info({ 0, 0, 100, 45 });
	m_elapsedTime = 0;
	m_tVector.fY = -20.f; // 위로 튀는 초기 속도
	m_szBuffer = L"";
	m_bAppear = false;
}

void CDmgText::Update()
{
	float dt = CTimeMgr::Get_Instance()->Get_Delta();
	if (!m_bAppear) {
		m_bAppear = true;
		m_OriginY = m_tInfo.fY;
	}
	m_tInfo.fY += m_tVector.fY * dt * 10;
	m_tVector.fY += GRAVITY;
	if (m_bAppear) {
		if (m_OriginY <= m_tInfo.fY) {
			m_bActive = false;
		}
	}
}

void CDmgText::Late_Update()
{
}
void CDmgText::Render(HDC _hDC)
{
	// 원래 상태 저장
	COLORREF oldColor = GetTextColor(_hDC);
	int oldBkMode = GetBkMode(_hDC);

	HFONT hFont = CFunction::CreateFontW(L"NotoSans-Medium", 24, true);
	HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);

	INFO_EX info = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	info.Update_INFO(m_tRect);

	SetTextColor(_hDC, RGB(178, 204, 255));
	SetBkMode(_hDC, TRANSPARENT);

	DrawText(_hDC, m_szBuffer.c_str(),
		static_cast<int>(m_szBuffer.length()),
		&m_tRect,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// 원래대로 복원
	SetTextColor(_hDC, oldColor);
	SetBkMode(_hDC, oldBkMode);
	SelectObject(_hDC, hOldFont);
	DeleteObject(hFont);
}



void CDmgText::Release()
{

}

void CDmgText::Set_Text(float dmg)
{
	m_szBuffer = to_wstring((int)dmg);
}
