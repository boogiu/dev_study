#include "pch.h"
#include "CTestScene.h"
#include "CStage.h"
#include "CTestStage.h"
#include "CResourceMgr.h"
#include "CCamera.h"
#include "CInfiniteObj.h"
#include "CObjMgr.h"

CTestScene::CTestScene()
	:m_NowStage(nullptr)
{
}

CTestScene::~CTestScene()
{
	Release();
}

void CTestScene::Initialize()
{
	CCamera::Get_Instance()->Set_Target(CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER));
	m_NowStage = new CTestStage;
	m_NowStage->Initialize();
}

void CTestScene::Update()
{
	m_NowStage->Update();
}

void CTestScene::Late_Update()
{
	m_NowStage->Late_Update();
}

void CTestScene::Render(HDC _hDC)
{
	auto pCam = CCamera::Get_Instance();
	SIZE sz = pCam->Get_CamSize();
	HDC hGroundDC = CResourceMgr::Get_Instance()->Find_Image(L"Ground");
	BitBlt(_hDC, 0, 0, sz.cx, sz.cy, hGroundDC, 0, 0, SRCCOPY);
	
	m_NowStage->Render(_hDC);
}

void CTestScene::Release()
{
	Safe_Delete<CStage*>(m_NowStage);
}

void CTestScene::CloseScene()
{
}
