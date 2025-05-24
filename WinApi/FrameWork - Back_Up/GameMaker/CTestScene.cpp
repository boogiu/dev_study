#include "pch.h"
#include "CTestScene.h"
#include "CStage.h"
#include "CTestStage.h"
#include "CResourceMgr.h"

CTestScene::CTestScene()
{
}

CTestScene::~CTestScene()
{
	Release();
}

void CTestScene::Initialize()
{

	m_NowStage = new CTestStage;
	m_NowStage->Initialize();
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Defualt_Back.bmp", L"Ground");

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
	HDC hGroundDC = CResourceMgr::Get_Instance()->Find_Image(L"Ground");;
	BitBlt(_hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);
	m_NowStage->Render(_hDC);
}

void CTestScene::Release()
{
	Safe_Delete<CStage*>(m_NowStage);
}
