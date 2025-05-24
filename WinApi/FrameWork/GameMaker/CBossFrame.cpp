#include "pch.h"
#include "CBossFrame.h"
#include "CResourceMgr.h"
#include "CIcon.h"
#include "CFunction.h"
#include "CCamera.h"

CBossFrame::CBossFrame()
	: m_bPahse2(false), Phase2Length(0.f), Phase1Length(0.f)
{
}

CBossFrame::~CBossFrame()
{
	Release();
}

void CBossFrame::Initialize()
{
	Phase2Length = 545.f;
	Phase1Length = 255.f;

	//------------------------------------------------------------------
	m_IconContainer.insert({ L"Top_Back", new CIcon });
	m_IconContainer[L"Top_Back"]->Initialize();
	m_IconContainer[L"Top_Back"]->LoadSprite(L"Ch2BossFirstPhase_Top_Back");
	INFO iconInfo = { WINCX/2, 725.f, 612, 108 };
	m_IconContainer[L"Top_Back"]->Set_Info(iconInfo);
	m_IconContainer[L"Top_Back"]->Get_INFO().SetFromTop(0);
	//------------------------------------------------------------------
	m_IconContainer.insert({ L"Bottom_Back", new CIcon });
	m_IconContainer[L"Bottom_Back"]->Initialize();
	m_IconContainer[L"Bottom_Back"]->LoadSprite(L"Ch2BossFirstPhase_Bottom_Back");
	iconInfo = { WINCX / 2, 725.f, 612, 108 };
	m_IconContainer[L"Bottom_Back"]->Set_Info(iconInfo);
	m_IconContainer[L"Bottom_Back"]->Get_INFO().SetFromTop(18);	
	//------------------------------------------------------------------
	m_IconContainer.insert({ L"HealthBar_Front", new CIcon });
	m_IconContainer[L"HealthBar_Front"]->Initialize();
	m_IconContainer[L"HealthBar_Front"]->LoadSprite(L"Boss_HealthBar_Front");
	iconInfo = { WINCX / 2, 725.f, 612, 108 };
	m_IconContainer[L"HealthBar_Front"]->Set_Info(iconInfo);
	m_IconContainer[L"HealthBar_Front"]->Get_INFO().SetFromTop(9);	
	
	//------------------------------------------------------------------
	m_IconContainer.insert({ L"BossA_HealthBar", new CIcon });
	m_IconContainer[L"BossA_HealthBar"]->Initialize();
	m_IconContainer[L"BossA_HealthBar"]->LoadSprite(L"BossHealthBar_FirstPhase");
	iconInfo = { WINCX / 2, 0, 255.f, 20.f};
	m_IconContainer[L"BossA_HealthBar"]->Set_Info(iconInfo);
	m_IconContainer[L"BossA_HealthBar"]->Get_INFO().SetFromTop(64);
	m_IconContainer[L"BossA_HealthBar"]->Get_INFO().SetFromRight((float)WINCX / 2 -19);

	//------------------------------------------------------------------
	m_IconContainer.insert({ L"BossB_HealthBar", new CIcon });
	m_IconContainer[L"BossB_HealthBar"]->Initialize();
	m_IconContainer[L"BossB_HealthBar"]->LoadSprite(L"BossHealthBar_FirstPhase");
	iconInfo = { WINCX / 2, 0, 255.f, 20.f };
	m_IconContainer[L"BossB_HealthBar"]->Set_Info(iconInfo);
	m_IconContainer[L"BossB_HealthBar"]->Get_INFO().SetFromTop(64);
	m_IconContainer[L"BossB_HealthBar"]->Get_INFO().SetFromLeft((float)WINCX / 2+ 19);

	//---------------------------------------------

	m_IconContainer.insert({ L"Top_Back2", new CIcon });
	m_IconContainer[L"Top_Back2"]->Initialize();
	m_IconContainer[L"Top_Back2"]->LoadSprite(L"Ch2BossSecondPhase_Top_Back");
	iconInfo = { WINCX / 2, 725.f, 612, 108 };
	m_IconContainer[L"Top_Back2"]->Set_Info(iconInfo);
	m_IconContainer[L"Top_Back2"]->Get_INFO().SetFromTop(0);

	//----------------------------/
	m_IconContainer.insert({ L"Bottom_Back2", new CIcon });
	m_IconContainer[L"Bottom_Back2"]->Initialize();
	m_IconContainer[L"Bottom_Back2"]->LoadSprite(L"Ch2BossSecondPhase_Bottom_Back");
	iconInfo = { WINCX / 2, 725.f, 612, 108 };
	m_IconContainer[L"Bottom_Back2"]->Set_Info(iconInfo);
	m_IconContainer[L"Bottom_Back2"]->Get_INFO().SetFromTop(18);
	//------------------------------------------------
	m_IconContainer.insert({ L"HealthBar_Front2", new CIcon });
	m_IconContainer[L"HealthBar_Front2"]->Initialize();
	m_IconContainer[L"HealthBar_Front2"]->LoadSprite(L"Ch2BossSecondPhase_Front");
	iconInfo = { WINCX / 2, 725.f, 612, 108 };
	m_IconContainer[L"HealthBar_Front2"]->Set_Info(iconInfo);
	m_IconContainer[L"HealthBar_Front2"]->Get_INFO().SetFromTop(9);
	//------------------------------------------------
	m_IconContainer.insert({ L"BossC_HealthBar", new CIcon });
	m_IconContainer[L"BossC_HealthBar"]->Initialize();
	m_IconContainer[L"BossC_HealthBar"]->LoadSprite(L"BossHealthBar_SecondPhase");
	iconInfo = { WINCX / 2, 0, Phase2Length, 20.f };
	m_IconContainer[L"BossC_HealthBar"]->Set_Info(iconInfo);
	m_IconContainer[L"BossC_HealthBar"]->Get_INFO().SetFromTop(64);
	m_IconContainer[L"BossC_HealthBar"]->Get_INFO().SetFromLeft((WINCX / 2) - (Phase2Length / 2));
}

void CBossFrame::Update()
{
	auto iter = m_IconContainer.begin();
	for (;iter != m_IconContainer.end(); ++iter) {
		(iter->second)->Update();
	}
}

void CBossFrame::Late_Update()
{
}

void CBossFrame::Render(HDC _hDC)
{
	if (!m_bPahse2) {
		m_IconContainer[L"Top_Back"]->Render(_hDC);
		m_IconContainer[L"Bottom_Back"]->Render(_hDC);
		m_IconContainer[L"HealthBar_Front"]->Render(_hDC);
		m_IconContainer[L"BossA_HealthBar"]->Render(_hDC);
		m_IconContainer[L"BossB_HealthBar"]->Render(_hDC);

		wstring m_szBuffer = L"레이아나 자매";
		// 원래 상태 저장
		COLORREF oldColor = GetTextColor(_hDC);
		int oldBkMode = GetBkMode(_hDC);

		HFONT hFont = CFunction::CreateFontW(L"NotoSans-Medium", 24, true);
		HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);

		RECT rc = { WINCX / 2-300, 20, WINCX / 2 + 300, 50};
		SetTextColor(_hDC, RGB(255, 255, 255));
		SetBkMode(_hDC, TRANSPARENT);

		DrawText(_hDC, m_szBuffer.c_str(),
			static_cast<int>(m_szBuffer.length()),
			&rc,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rc = { WINCX / 2 - 300,80, WINCX / 2 + 300, 120 };
		 m_szBuffer = L"황금 갈기 기사단 부관";

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
	else {
		m_IconContainer[L"Top_Back2"]->Render(_hDC);
		m_IconContainer[L"Bottom_Back2"]->Render(_hDC);
		m_IconContainer[L"HealthBar_Front2"]->Render(_hDC);
		m_IconContainer[L"BossC_HealthBar"]->Render(_hDC);

		wstring m_szBuffer = L"각성한 레이아나";
		// 원래 상태 저장
		COLORREF oldColor = GetTextColor(_hDC);
		int oldBkMode = GetBkMode(_hDC);

		HFONT hFont = CFunction::CreateFontW(L"NotoSans-Medium", 24, true);
		HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);

		RECT rc = { WINCX / 2 - 300, 20, WINCX / 2 + 300, 50 };
		SetTextColor(_hDC, RGB(255, 255, 255));
		SetBkMode(_hDC, TRANSPARENT);

		DrawText(_hDC, m_szBuffer.c_str(),
			static_cast<int>(m_szBuffer.length()),
			&rc,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rc = { WINCX / 2 - 300,80, WINCX / 2 + 300, 120 };
		m_szBuffer = L"황금 갈기 기사단 부관";

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
}

void CBossFrame::Release()
{
	auto iter = m_IconContainer.begin();
	for (;iter != m_IconContainer.end(); ++iter) {
		Safe_Delete<CIcon*>(iter->second);
	}
	m_IconContainer.clear();
}

void CBossFrame::CheckBossAHP(float hpRatio)
{
	float nowBarLength = Phase1Length * (hpRatio);
	INFO_EX& barInfo = m_IconContainer[L"BossA_HealthBar"]->Get_INFO();
	barInfo.offsetLeft = nowBarLength / 2.f;
	barInfo.offsetRight = nowBarLength / 2.f;
	barInfo.SetFromRight(((float)WINCX / 2 - 19));
}

void CBossFrame::CheckBossBHP(float hpRatio)
{
	float nowBarLength = Phase1Length * (hpRatio);
	INFO_EX& barInfo = m_IconContainer[L"BossB_HealthBar"]->Get_INFO();
	barInfo.offsetLeft = nowBarLength / 2.f;
	barInfo.offsetRight = nowBarLength / 2.f;
	barInfo.SetFromLeft(((float)WINCX / 2 + 19));
}

void CBossFrame::CheckBossCHP(float hpRatio)
{
	float nowBarLength = Phase2Length * (hpRatio);
	INFO_EX& barInfo = m_IconContainer[L"BossC_HealthBar"]->Get_INFO();
	barInfo.offsetLeft = nowBarLength / 2.f;
	barInfo.offsetRight = nowBarLength / 2.f;
	barInfo.SetFromLeft((WINCX / 2) - (Phase2Length / 2));
}

void CBossFrame::ChangePhase()
{
	m_bPahse2 = true;
}
