#include "pch.h"
#include "CSceneMgr.h"
#include "CStageScene.h"
#include "CSceneFactory.h"

CSceneMgr::CSceneMgr()
	: m_eNowScene(SCENE_ID::STAGE), m_vecScenes(static_cast<int>(SCENE_ID::END), nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Initialize()
{
	m_vecScenes[static_cast<int>(SCENE_ID::STAGE)] = CSceneFactory<CStageScene>::CreateScene();
	
	for (CScene* scene : m_vecScenes) {
		if(scene)
		scene->Initialize();
	}
}

void CSceneMgr::Update()
{
	if (m_vecScenes[static_cast<int>(m_eNowScene)]) {
		m_vecScenes[static_cast<int>(m_eNowScene)]->Update();
	}
}

void CSceneMgr::Late_Update()
{
	if (m_vecScenes[static_cast<int>(m_eNowScene)]) {
		m_vecScenes[static_cast<int>(m_eNowScene)]->Late_Update();
	}

}

void CSceneMgr::Render(HDC _hDC)
{
	Rectangle(_hDC, 0, 0, WINCX, WINCY);

	if (m_vecScenes[static_cast<int>(m_eNowScene)]) {
		m_vecScenes[static_cast<int>(m_eNowScene)]->Render(_hDC);
	}
}

void CSceneMgr::Release()
{
	for (CScene* scene : m_vecScenes) {
		if (scene)
		Safe_Delete<CScene*>(scene);
	}
}

void CSceneMgr::ChangeScene(SCENE_ID _to, SCENE_ID _from)
{
	m_vecScenes[static_cast<int>(_to)]->SetPlayer(
		m_vecScenes[static_cast<int>(_from)]->GetPlayer()
	);

	m_eNowScene = _from;
}
