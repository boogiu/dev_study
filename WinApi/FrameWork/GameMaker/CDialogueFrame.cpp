#include "pch.h"
#include "CDialogueFrame.h"
#include "CIcon.h"
#include "CTimeMgr.h"
#include "CFunction.h"
#include "CCamera.h"
#include "CResourceMgr.h"

CDialogueFrame::CDialogueFrame()
{
}

CDialogueFrame::~CDialogueFrame()
{
	Release();
}

void CDialogueFrame::Initialize()
{
	INFO iconInfo = { WINCX, WINCY - 150, WINCX-100, 200 };

	m_IconContainer.insert({ L"Dial_Frame", new CIcon });
	m_IconContainer[L"Dial_Frame"]->Initialize();
	m_IconContainer[L"Dial_Frame"]->LoadSprite(L"Popup_Frame");
	m_IconContainer[L"Dial_Frame"]->Set_Info(iconInfo);
	m_IconContainer[L"Dial_Frame"]->Get_INFO().SetFromBottom(WINCY-50);
	m_IconContainer[L"Dial_Frame"]->Get_INFO().SetFromLeft(WINCX);

	iconInfo = { WINCX, WINCY/2, 300, 300 };
	m_IconContainer.insert({ L"NPC", new CIcon });
	m_IconContainer[L"NPC"]->Initialize();
	m_IconContainer[L"NPC"]->LoadSprite(L"NPC_TwinOgre");
	m_IconContainer[L"NPC"]->Set_Info(iconInfo);
	m_IconContainer[L"NPC"]->Get_INFO().SetFromBottom(WINCY - 150);
	m_IconContainer[L"NPC"]->Get_INFO().SetFromRight(WINCX);	
		
	iconInfo = { WINCX, WINCY -250, 150, 50 };
	m_IconContainer.insert({ L"Dialogue_Name", new CIcon });
	m_IconContainer[L"Dialogue_Name"]->Initialize();
	m_IconContainer[L"Dialogue_Name"]->LoadSprite(L"Dialogue_Name");
	m_IconContainer[L"Dialogue_Name"]->Set_Info(iconInfo);
	m_IconContainer[L"Dialogue_Name"]->Get_INFO().SetFromBottom(WINCY - 220);
	m_IconContainer[L"Dialogue_Name"]->Get_INFO().SetFromRight(WINCX);
	DialCount = 0;
}

void CDialogueFrame::Update()
{
	auto iter = m_IconContainer.begin();
	for (;iter != m_IconContainer.end(); ++iter) {
		(iter->second)->Update();
	}
	MoveTo(L"Dial_Frame", 50.f);
	MoveTo(L"NPC", WINCX-300.f);
	MoveTo(L"Dialogue_Name", 80.f);
}

void CDialogueFrame::Late_Update()
{
}

void CDialogueFrame::Render(HDC _hDC)
{
	m_IconContainer[L"NPC"]->Render(_hDC);
	m_IconContainer[L"Dial_Frame"]->Render(_hDC);
	m_IconContainer[L"Dialogue_Name"]->Render(_hDC);
	RenderName(_hDC);
	RenderScript(_hDC);
}

void CDialogueFrame::Release()
{
	auto iter = m_IconContainer.begin();
	for (;iter != m_IconContainer.end(); ++iter) {
		Safe_Delete<CIcon*>(iter->second);
	}
	m_IconContainer.clear();
}

void CDialogueFrame::MoveTo(const TCHAR* Key, float Dest)
{
	auto it = m_IconContainer.find(Key);
	if (it == m_IconContainer.end() || !(it->second)) return;

	UI_INFO& frameInfo = it->second->Get_INFO();
	float speed = 3600.f;
	float deltaX = speed * CTimeMgr::Get_Instance()->Get_Delta();
	float newX = frameInfo.LT().x - deltaX;

	if (newX < Dest)
		newX = Dest;

	frameInfo.SetFromLeft(newX);
}

void CDialogueFrame::RenderName(HDC _hDC)
{
	// 원래 상태 저장
	COLORREF oldColor = GetTextColor(_hDC);
	int oldBkMode = GetBkMode(_hDC);

	HFONT hFont = CFunction::CreateFontW(L"NotoSans-Medium", 24, true);
	HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);

	RECT rc;
	m_IconContainer[L"Dialogue_Name"]->Get_INFO().Update_INFO(rc);

	SetTextColor(_hDC, RGB(255, 255, 255));
	SetBkMode(_hDC, TRANSPARENT);
	wstring m_szBuffer = L"오우거 보부상";

	DrawText(_hDC, m_szBuffer.c_str(),
		static_cast<int>(m_szBuffer.length()),
		&rc,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	// 원래대로 복원
	SetTextColor(_hDC, oldColor);
	SetBkMode(_hDC, oldBkMode);
	SelectObject(_hDC, hOldFont);
	DeleteObject(hFont);
}
void CDialogueFrame::RenderScript(HDC _hDC)
{
	static int prevDialCount = 0; // 이전 DialCount를 기억

	DialCount += CTimeMgr::Get_Instance()->Get_Delta() * 20;
	wstring m_szBuffer = L"오...스컬.....\n나 보물을 빼앗겼다....레이아나 자매에게 뺏긴 보물을 되찾아줘라...\n이게 도움이 되면 좋겠다....";

	float MyDialCount = DialCount/2; // 출력 속도 3배 느리게

	int currentCharCount = static_cast<int>(MyDialCount);
	if (currentCharCount > prevDialCount && currentCharCount <= m_szBuffer.length())
	{
		wchar_t lastChar = m_szBuffer[currentCharCount - 1];
		if (lastChar != L'.')
		{
			CResourceMgr::Get_Instance()->PlaySoundW(L"Dot_Sound", 0.1f);
		}
		prevDialCount = currentCharCount;
	}

	// 원래 상태 저장
	COLORREF oldColor = GetTextColor(_hDC);
	int oldBkMode = GetBkMode(_hDC);

	HFONT hFont = CFunction::CreateFontW(L"NotoSans-Medium", 24, true);
	HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);
	INFO_EX info = m_IconContainer[L"Dial_Frame"]->Get_INFO();

	info.Add_OffsetX(-40.f, -20.f);
	info.Add_OffsetY(-40.f, -20.f);
	RECT rc;
	info.Update_INFO(rc);

	SetTextColor(_hDC, RGB(122, 103, 75));
	SetBkMode(_hDC, TRANSPARENT);

	wstring limitedText = m_szBuffer.substr(0, currentCharCount);

	DrawText(_hDC, limitedText.c_str(),
		static_cast<int>(limitedText.length()),
		&rc,
		DT_LEFT);

	// 원래대로 복원
	SetTextColor(_hDC, oldColor);
	SetBkMode(_hDC, oldBkMode);
	SelectObject(_hDC, hOldFont);
	DeleteObject(hFont);
}

