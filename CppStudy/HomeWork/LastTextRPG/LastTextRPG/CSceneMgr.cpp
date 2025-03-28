#include "pch.h"
#include "CSceneMgr.h"
#include "CGameMgr.h"
#include "CScene.h"
#include "Scenes.h"

CSceneMgr::CSceneMgr()
	:nowScene(SCENE::MAIN), m_SceneArr{ static_cast<int>(SCENE::END) }
{
}

CSceneMgr::~CSceneMgr()
{
}

void CSceneMgr::Initialize()
{

	m_SceneArr[static_cast<int>(SCENE::MAIN)] = new CMainScene;
	m_SceneArr[static_cast<int>(SCENE::CLASS)] = new CClassScene;
	m_SceneArr[static_cast<int>(SCENE::VILLAGE)] = new CVillageScene;
	m_SceneArr[static_cast<int>(SCENE::SHOP)] = new CShopScene;
	m_SceneArr[static_cast<int>(SCENE::MAZE)] = new CMazeScene;
	m_SceneArr[static_cast<int>(SCENE::BATTLE)] = new CBattleScene;

	if (m_SceneArr.size() != 0) {
		for (size_t i = 0; i < m_SceneArr.size(); ++i) {
			if (m_SceneArr[i]) {
				m_SceneArr[i]->Initialize();
			}
		}
	}

}

void CSceneMgr::Release()
{
	//æ¿µÈ ªË¡¶
	if (m_SceneArr.size() != 0) {
		for (size_t i = 0; i < m_SceneArr.size(); ++i) {
			if (m_SceneArr[i]) {
				m_SceneArr[i]->Release();
				SAFE_DELETE(m_SceneArr[i]);
			}
		}
	}
}

void CSceneMgr::Update()
{
	if (nowScene == SCENE::END) {
		return;
	}
	if (m_SceneArr[static_cast<int>(nowScene)]) {
		m_SceneArr[static_cast<int>(nowScene)]->Update();
	}
	else {
		wcout << "æ¿ æ˜µ•¿Ã∆Æ Ω«∆–" << endl;
	}
}

void CSceneMgr::Render()
{
	if (nowScene == SCENE::END) {
		return;
	}
	if (m_SceneArr[static_cast<int>(nowScene)]) {
		m_SceneArr[static_cast<int>(nowScene)]->Render();
	}
	else {
		wcout << "æ¿ ∑ª¥ı Ω«∆–" << endl;
	}
}

void CSceneMgr::ChangeScene(SCENE _scene)
{
	nowScene = _scene;
}
