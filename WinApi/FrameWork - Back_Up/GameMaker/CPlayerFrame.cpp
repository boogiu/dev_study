#include "pch.h"
#include "CPlayerFrame.h"
#include "CResourceMgr.h"
#include "CIcon.h"

CPlayerFrame::CPlayerFrame()
{
	//나중에 스컬로 뺄 것들


	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Player_Frame.bmp", L"Player_Frame");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Player_HealthBar.bmp", L"Player_HealthBar");

	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/S_Btn.bmp", L"S_Btn");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/A_Btn.bmp", L"A_Btn");

	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Skill2_Frame.bmp", L"Skill2_Frame");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Subskull_Frame.bmp", L"Subskull_Frame");
}

CPlayerFrame::~CPlayerFrame()
{
	Release();
}

void CPlayerFrame::Initialize()
{
	INFO myInfo = { 250.f ,750.f,450.f,180.f };
	m_tInfo.Translate_Info(myInfo);

	m_IconContainer.insert({ L"Skul_ICON", new CIcon });
	m_IconContainer[L"Skul_ICON"]->Initialize();
	m_IconContainer[L"Skul_ICON"]->LoadSprite(L"Skul_ICON");
	INFO iconInfo = {100.f, 725.f, 130.f, 130.f};
	m_IconContainer[L"Skul_ICON"]->Set_Info(iconInfo);

	m_IconContainer.insert({ L"Base_Skill_icon", new CIcon });
	m_IconContainer[L"Base_Skill_icon"]->Initialize();
	m_IconContainer[L"Base_Skill_icon"]->LoadSprite(L"Base_Skill_icon");
	INFO Skill = { 212.f, 735.f, 69.f, 69.f };
	m_IconContainer[L"Base_Skill_icon"]->Set_Info(Skill);	
	
	m_IconContainer.insert({ L"Base_Skill2_icon", new CIcon });
	m_IconContainer[L"Base_Skill2_icon"]->Initialize();
	m_IconContainer[L"Base_Skill2_icon"]->LoadSprite(L"Base_Skill2_icon");
	INFO Skill2 = { 291.f, 730.f, 69.f, 69.f };
	m_IconContainer[L"Base_Skill2_icon"]->Set_Info(Skill2);

	m_IconContainer.insert({ L"Player_HealthBar", new CIcon });
	m_IconContainer[L"Player_HealthBar"]->Initialize();
	m_IconContainer[L"Player_HealthBar"]->LoadSprite(L"Player_HealthBar");
	INFO Bar = { 140.f, 794.f, 0.f, 25.f };
	m_IconContainer[L"Player_HealthBar"]->Set_Info(Bar);
	m_IconContainer[L"Player_HealthBar"]->Get_INFO().Add_OffsetX(0.f, 315.f);


	INFO A_Btn = { 212.f, 694.f, 34.f, 34.f };
	m_IconContainer.insert({ L"A_Btn", new CIcon });
	m_IconContainer[L"A_Btn"]->LoadSprite(L"A_Btn");
	m_IconContainer[L"A_Btn"]->Set_Info(A_Btn);

	INFO S_Btn = { 290.f, 694.f, 34.f, 34.f };
	m_IconContainer.insert({ L"S_Btn", new CIcon });
	m_IconContainer[L"S_Btn"]->LoadSprite(L"S_Btn");
	m_IconContainer[L"S_Btn"]->Set_Info(S_Btn);
	
	INFO Skill2_Frame = { 290.f, 736.f, 91.f, 91.f };
	m_IconContainer.insert({ L"Skill2_Frame", new CIcon });
	m_IconContainer[L"Skill2_Frame"]->LoadSprite(L"Skill2_Frame");
	m_IconContainer[L"Skill2_Frame"]->Set_Info(Skill2_Frame);

	INFO Subskull_Frame = { 60.f, 800.f, 80.f, 80.f };
	m_IconContainer.insert({ L"Subskull_Frame", new CIcon });
	m_IconContainer[L"Subskull_Frame"]->LoadSprite(L"Subskull_Frame");
	m_IconContainer[L"Subskull_Frame"]->Set_Info(Subskull_Frame);
}

void CPlayerFrame::Update()
{
	m_tInfo.Update_INFO(m_tRect);
	auto iter = m_IconContainer.begin();
	for (;iter != m_IconContainer.end(); ++iter) {
		(iter->second)->Update();
	}
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
