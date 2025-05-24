#include "pch.h"
#include "CPlayerFrame.h"
#include "CResourceMgr.h"
#include "CIcon.h"
#include "CObject.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CSkul.h"
CPlayerFrame::CPlayerFrame() : maxBarLength(207.f), nowBarLength(0.f)
{
	//나중에 스컬로 뺄 것들

}

CPlayerFrame::~CPlayerFrame()
{
	Release();
}

void CPlayerFrame::Initialize()
{
	INFO tmp = { WINCX / 6,WINCY * 6 / 7,300,120 };
	m_tInfo.Translate_Info(tmp);
	m_tInfo.SetFromBottom(WINCY);
	m_tInfo.SetFromLeft(0);

	//------------------------------------------------------------------
	m_IconContainer.insert({ L"Skul_ICON", new CIcon });
	m_IconContainer[L"Skul_ICON"]->Initialize();
	m_IconContainer[L"Skul_ICON"]->LoadSprite(L"Skul_ICON");
	INFO iconInfo = { 100.f, 725.f, 80.f, 80.f };
	m_IconContainer[L"Skul_ICON"]->Set_Info(iconInfo);
	m_IconContainer[L"Skul_ICON"]->Get_INFO().SetFromBottom(WINCY - 37);
	m_IconContainer[L"Skul_ICON"]->Get_INFO().SetFromLeft(10);


	m_IconContainer.insert({ L"Base_Skill_icon", new CIcon });
	m_IconContainer[L"Base_Skill_icon"]->Initialize();
	m_IconContainer[L"Base_Skill_icon"]->LoadSprite(L"Base_Skill_icon");
	INFO Skill = { 212.f, 735.f, 43.f, 43.f };
	m_IconContainer[L"Base_Skill_icon"]->Set_Info(Skill);
	m_IconContainer[L"Base_Skill_icon"]->Get_INFO().SetFromBottom(WINCY - 49);
	m_IconContainer[L"Base_Skill_icon"]->Get_INFO().SetFromLeft(103);


	m_IconContainer.insert({ L"Base_Skill2_icon", new CIcon });
	m_IconContainer[L"Base_Skill2_icon"]->Initialize();
	m_IconContainer[L"Base_Skill2_icon"]->LoadSprite(L"Base_Skill2_icon");
	INFO Skill2 = { 212.f, 735.f, 43.f, 43.f };
	m_IconContainer[L"Base_Skill2_icon"]->Set_Info(Skill2);
	m_IconContainer[L"Base_Skill2_icon"]->Get_INFO().SetFromBottom(WINCY - 49);
	m_IconContainer[L"Base_Skill2_icon"]->Get_INFO().SetFromLeft(161);

	INFO Skill2_Frame = { 290.f, 736.f, 61.f, 61.f };
	m_IconContainer.insert({ L"Skill2_Frame", new CIcon });
	m_IconContainer[L"Skill2_Frame"]->LoadSprite(L"Skill2_Frame");
	m_IconContainer[L"Skill2_Frame"]->Set_Info(Skill2_Frame);
	m_IconContainer[L"Skill2_Frame"]->Get_INFO().SetFromBottom(WINCY - 39);
	m_IconContainer[L"Skill2_Frame"]->Get_INFO().SetFromLeft(151);

	m_IconContainer.insert({ L"Player_HealthBar", new CIcon });
	m_IconContainer[L"Player_HealthBar"]->Initialize();
	m_IconContainer[L"Player_HealthBar"]->LoadSprite(L"Player_HealthBar");
	INFO Bar = { 140.f, 794.f, maxBarLength , 17.f };
	m_IconContainer[L"Player_HealthBar"]->Set_Info(Bar);
	m_IconContainer[L"Player_HealthBar"]->Get_INFO().SetFromBottom(WINCY - 23);
	m_IconContainer[L"Player_HealthBar"]->Get_INFO().SetFromLeft(78);


	INFO A_Btn = { 212.f, 694.f, 19.f, 19.f };
	m_IconContainer.insert({ L"A_Btn", new CIcon });
	m_IconContainer[L"A_Btn"]->LoadSprite(L"A_Btn");
	m_IconContainer[L"A_Btn"]->Set_Info(A_Btn);
	m_IconContainer[L"A_Btn"]->Get_INFO().SetFromBottom(WINCY - 87);
	m_IconContainer[L"A_Btn"]->Get_INFO().SetFromLeft(115);

	INFO S_Btn = { 290.f, 694.f,  19.f, 19.f };
	m_IconContainer.insert({ L"S_Btn", new CIcon });
	m_IconContainer[L"S_Btn"]->LoadSprite(L"S_Btn");
	m_IconContainer[L"S_Btn"]->Set_Info(S_Btn);
	m_IconContainer[L"S_Btn"]->Get_INFO().SetFromBottom(WINCY - 87);
	m_IconContainer[L"S_Btn"]->Get_INFO().SetFromLeft(173);

	INFO Subskull_Frame = { 60.f, 800.f, 50.f, 50.f };
	m_IconContainer.insert({ L"Subskull_Frame", new CIcon });
	m_IconContainer[L"Subskull_Frame"]->LoadSprite(L"Subskull_Frame");
	m_IconContainer[L"Subskull_Frame"]->Set_Info(Subskull_Frame);
	m_IconContainer[L"Subskull_Frame"]->Get_INFO().SetFromBottom(WINCY - 7);
	m_IconContainer[L"Subskull_Frame"]->Get_INFO().SetFromLeft(9);

	CPlayer* castPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER));
	INFO Subskull_ICON = { 60.f, 800.f, 38.f, 38.f };
	m_IconContainer.insert({ L"Subskull_ICON", new CIcon });
	m_IconContainer[L"Subskull_ICON"]->LoadSprite(L"Ent_ICON");
	m_IconContainer[L"Subskull_ICON"]->Set_Info(Subskull_ICON);
	m_IconContainer[L"Subskull_ICON"]->Get_INFO().SetFromBottom(WINCY - 13);
	m_IconContainer[L"Subskull_ICON"]->Get_INFO().SetFromLeft(16);
}

void CPlayerFrame::Update()
{
	m_tInfo.Update_INFO(m_tRect);
	auto iter = m_IconContainer.begin();
	for (;iter != m_IconContainer.end(); ++iter) {
		(iter->second)->Update();
	}

	CPlayer* castPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER));
	float hpRatio = castPlayer->HpPercent() / 100.f;
	nowBarLength = maxBarLength * hpRatio;

	INFO_EX& barInfo = m_IconContainer[L"Player_HealthBar"]->Get_INFO();
	barInfo.offsetLeft = nowBarLength / 2.f;
	barInfo.offsetRight = nowBarLength / 2.f;
	barInfo.SetFromLeft(78);


	m_IconContainer[L"Base_Skill_icon"]->LoadSprite(castPlayer->Get_Skul()->Get_SkillA_Icon().c_str());
	m_IconContainer[L"Base_Skill2_icon"]->LoadSprite(castPlayer->Get_Skul()->Get_SkillB_Icon().c_str());
	if (castPlayer->Get_SubSkul()) {
		m_IconContainer[L"Subskull_ICON"]->LoadSprite(castPlayer->Get_SubSkul()->Get_Skull_Icon().c_str());
	}
	m_IconContainer[L"Skul_ICON"]->LoadSprite(castPlayer->Get_Skul()->Get_Skull_Icon().c_str());
}

void CPlayerFrame::Late_Update()
{
}

void CPlayerFrame::Render(HDC _hDC)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(L"Player_Frame");
	GdiTransparentBlt(
		_hDC,
		m_tRect.left,
		m_tRect.top,
		m_tInfo.X_Size(), m_tInfo.Y_Size(),
		memDC,
		0, 0,
		168, 66,
		HOLLOW
	);

	m_IconContainer[L"Player_HealthBar"]->Render(_hDC);
	m_IconContainer[L"Skul_ICON"]->Render(_hDC);

	m_IconContainer[L"Skill2_Frame"]->Render(_hDC);
	m_IconContainer[L"Subskull_Frame"]->Render(_hDC);

	CPlayer* castPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER));
	if (castPlayer->Get_SubSkul()) {
		m_IconContainer[L"Subskull_ICON"]->Render(_hDC);
	}

	m_IconContainer[L"Base_Skill_icon"]->Render(_hDC);
	m_IconContainer[L"Base_Skill2_icon"]->Render(_hDC);

	m_IconContainer[L"A_Btn"]->Render(_hDC);
	m_IconContainer[L"S_Btn"]->Render(_hDC);
}

void CPlayerFrame::Release()
{
	auto iter = m_IconContainer.begin();
	for (;iter != m_IconContainer.end(); ++iter) {
		Safe_Delete<CIcon*>(iter->second);
	}
	m_IconContainer.clear();
}
