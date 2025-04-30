#include "pch.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CTestScene.h"
CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() 
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Initialize()
{
	m_NowScene = new CTestScene;
	m_NowScene->Initialize();
}

void CSceneMgr::Update()
{
}

void CSceneMgr::Late_Update()
{

}

void CSceneMgr::Render(HDC _hDC)
{
	m_NowScene->Render(_hDC);
}

void CSceneMgr::Release()
{
}
