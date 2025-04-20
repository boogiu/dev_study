#include "pch.h"
#include "CSceneMgr.h"
#include "Scenes.h"

CSceneMgr* CSceneMgr::m_SceneMgr = nullptr;


CSceneMgr::CSceneMgr()
	:m_SceneID{ L"MAIN" }
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Intialize()
{
	RegisterScene<CMainScene>(L"MAIN");
}

void CSceneMgr::Update()
{
	m_umapScene[m_SceneID]->Update();
}

void CSceneMgr::Late_Update()
{
	m_umapScene[m_SceneID]->Late_Update();
}

void CSceneMgr::Render(HDC _hDC)
{
	m_umapScene[m_SceneID]->Render(_hDC);
}

void CSceneMgr::Release()
{
	auto iter = m_umapScene.begin();
	for (;iter != m_umapScene.end(); ++iter) {
		Safe_Delete<CScene*>(iter->second);
	}
	m_umapScene.clear();
}

void CSceneMgr::ChangeScene(wstring _sceneID)
{
	m_SceneID = _sceneID;
}

bool CSceneMgr::IsSceneIDValid(wstring _sceneID)
{
	return m_umapScene.find(_sceneID)==m_umapScene.end();
}
