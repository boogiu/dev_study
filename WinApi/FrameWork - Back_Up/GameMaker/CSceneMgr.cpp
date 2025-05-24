#include "pch.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CTestScene.h"
#include "CResourceMgr.h"
CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() : m_NowScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Initialize()
{
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/tile_32_4.bmp", L"Tile");

	m_NowScene = new CTestScene;
	m_NowScene->Initialize();

}

void CSceneMgr::Update()
{
	m_NowScene->Update();

}

void CSceneMgr::Late_Update()
{
	m_NowScene->Late_Update();
}

void CSceneMgr::Render(HDC _hDC)
{
	m_NowScene->Render(_hDC);

}

void CSceneMgr::Release()
{
	Safe_Delete<CScene*>(m_NowScene);
}
