#include "pch.h"
#include "CUIMgr.h"
#include "CBase_UI.h"
#include "CPlayerFrame.h"
#include "CCamera.h"
#include "CObjMgr.h"
#include "CObject.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
	:m_pTarget(nullptr)
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
	m_pTarget = CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER);
}

void CUIMgr::Update()
{
	for (CBase_UI* ui : m_UI_List) {
		ui->Update();
	}
}

void CUIMgr::Late_Update()
{
	for (CBase_UI* ui : m_UI_List) {
		ui->Late_Update();
	}
}

void CUIMgr::Render(HDC _hDC)
{
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
