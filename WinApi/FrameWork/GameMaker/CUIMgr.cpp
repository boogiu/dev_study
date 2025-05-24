#include "pch.h"
#include "CUIMgr.h"
#include "CBase_UI.h"
#include "CPlayerFrame.h"
#include "CCamera.h"
#include "CObjMgr.h"
#include "CObject.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
	:m_pTarget(nullptr), m_bShow(false)
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Initialize()
{
	CBase_UI* tmp = new CPlayerFrame;
	tmp->Initialize();
	m_UI_List.push_back(tmp);
}

void CUIMgr::Update()
{
	if (!m_bShow) return;
	for (CBase_UI* ui : m_UI_List) {
		ui->Update();
	}
}

void CUIMgr::Late_Update()
{
	if (!m_bShow) return;

	for (auto iter = m_UI_List.begin(); iter != m_UI_List.end(); ) {
		CBase_UI* ui = *iter;

		if (!ui->Get_Actvie()) {
			Safe_Delete<CBase_UI*>(ui);
			iter = m_UI_List.erase(iter); // erase 후 다음 요소로 이동
		}
		else {
			ui->Late_Update();
			++iter;
		}
	}
}


void CUIMgr::Render(HDC _hDC)
{
	if (!m_bShow) return;

	for (CBase_UI* ui : m_UI_List) {
		ui->Render(_hDC);
	}

}

void CUIMgr::Release()
{
	for (CBase_UI* ui : m_UI_List) {
		Safe_Delete<CBase_UI*>(ui);
	}
}

void CUIMgr::UI_Set(bool Show)
{
	m_bShow = Show;
	m_pTarget = CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER);
}
