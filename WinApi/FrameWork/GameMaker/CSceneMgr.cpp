#include "pch.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CResourceMgr.h"
#include "Scenes.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() : m_NowScene(LOGO)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Initialize()
{
	m_SceneContainer[LOGO] = new CLogoScene;
	m_SceneContainer[LOGO]->Initialize();
	m_SceneContainer[BATTLE] = new CBattleScene;

}

void CSceneMgr::Update()
{
	m_SceneContainer[m_NowScene]->Update();
	
}

void CSceneMgr::Late_Update()
{
	m_SceneContainer[m_NowScene]->Late_Update();

}

void CSceneMgr::Render(HDC _hDC)
{
	m_SceneContainer[m_NowScene]->Render(_hDC);
}

void CSceneMgr::Release()
{
	auto iter = m_SceneContainer.begin();
	for (;iter != m_SceneContainer.end(); ++iter) {
		Safe_Delete<CScene*>(iter->second);
	}
	m_SceneContainer.clear();
}

void CSceneMgr::Change_Scene(SCENE_TAG nextScene)
{
	if (m_NowScene == nextScene) return;
	if (m_SceneContainer[nextScene]) {
		m_SceneContainer[m_NowScene]->CloseScene();
		CObjMgr::Get_Instance()->ClearObject();
		m_NowScene = nextScene;
	}
	CCamera::Get_Instance()->MoveTo({ 0,50 });
	m_SceneContainer[m_NowScene]->Initialize();
}
