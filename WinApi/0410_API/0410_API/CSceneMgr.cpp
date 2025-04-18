#include "pch.h"
#include "CSceneMgr.h"
#include "Scenes.h"
#include "CBorder.h"

CSceneMgr::CSceneMgr()
	: m_eNowScene(SCENE::MAIN), m_vecScene(static_cast<int>(SCENE::END),nullptr) , m_Border(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Initialize()
{
	m_vecScene[static_cast<int>(SCENE::MAIN)] = new CMainScene;
	m_vecScene[static_cast<int>(SCENE::STAGE)] = new CStageScene;
	
	m_Border = new CBorder;
	m_Border->Initialize();

	for (CScene* scene : m_vecScene) {
		if (scene) {
			scene->Initialize();
		}
	}
}

void CSceneMgr::Update()
{
	if (m_eNowScene == SCENE::END) {
		return;
	}
	m_Border->Update();
	m_vecScene[static_cast<int>(m_eNowScene)]->Update();

}

void CSceneMgr::Render(HDC _hDC)
{
	m_Border->Render(_hDC);
	m_vecScene[static_cast<int>(m_eNowScene)]->Render(_hDC);

}

void CSceneMgr::Release()
{
	for (CScene* scene : m_vecScene) {
		Safe_Delete<CScene*>(scene);
	}
	Safe_Delete<CBorder*>(m_Border);

}

void CSceneMgr::ChangeScene(SCENE _nextScene)
{
	m_eNowScene = _nextScene;
}
