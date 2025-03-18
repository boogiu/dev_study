#include "CGameMgr.h"
#include "pch.h"

#include "CBaseScene.h"
#include "CStartScene.h"
#include "CMainScene.h"
#include "CFieldScene.h"

#include "CPlayer.h"

CGameMgr::CGameMgr()
	:GameOn(true), m_SceneArr(nullptr), m_nowScene(SCENE_ENUM::START), m_player(nullptr)
{}

CGameMgr::~CGameMgr()
{

}

void CGameMgr::Init()
{
	if (m_SceneArr) return;
	if (m_player) return;

	m_SceneArr = new CBaseScene*[SCENE_ENUM::END]; // 씬을 담는 배열 할당. //각 인덱스 별로 씬을 생성-> 할당 과정 필요.
	
	m_SceneArr[START] = new CStartScene;
	m_SceneArr[MAIN] = new CMainScene;
	m_SceneArr[FIELD] = new CFieldScene;

	for (int i = 0; i < SCENE_ENUM::END; ++i) {
		m_SceneArr[i]->Init();
	}

	m_player = new CPlayer;
	m_player->Init();
}

bool CGameMgr::Render()
{
	if (m_nowScene == SCENE_ENUM::END) return false;

	m_SceneArr[m_nowScene]->Render();
	system("cls");
	
	return true;
}

void CGameMgr::Release()
{
	if (!m_SceneArr) return;
	if (!m_player) return;

	for (int i = 0; i < SCENE_ENUM::END; ++i) {
		if (!m_SceneArr[i]) continue;

		m_SceneArr[i]->Release();
		SAFE_DELETE(m_SceneArr[i]);
	}
	SAFE_DELETE_ARR(m_SceneArr);
	
	m_player->Release();
	SAFE_DELETE(m_player);
}
