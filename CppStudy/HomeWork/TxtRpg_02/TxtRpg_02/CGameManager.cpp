#include "pch.h"
#include "CGameManager.h"
#include "CScene.h"
#include "CMainScene.h"
#include "CVillage.h"
#include "CField.h"
#include "CPlayer.h"
#include "CDataBase.h"

E_SCENE CGameManager::m_nowScene = E_SCENE::START;

CGameManager::CGameManager()
	:  m_pPlayer(nullptr), m_SceneArr(nullptr),m_DataBase(nullptr)
{
}

CGameManager::~CGameManager()
{
}

void CGameManager::Init()
{
	if (!m_pPlayer) {
		m_pPlayer = new CPlayer();
		m_pPlayer->Init();
	}

	if (!m_SceneArr) {
		m_SceneArr = new CScene * [E_SCENE::END];
		//°¢ ÀÎµ¦½ºº°·Î ¾À °´Ã¼ ÇÒ´çÇØÁÖ±â.
		m_SceneArr[E_SCENE::START] = new CMainScene;
		m_SceneArr[E_SCENE::VILLAGE] = new CVillage;
		m_SceneArr[E_SCENE::FIELD] = new CField;
	}
	if (!m_DataBase) {
		m_DataBase = new CDataBase;
		m_DataBase->Init();
	}
	for (int i = 0; i < E_SCENE::END; ++i) {
		if (m_SceneArr[i]) {
			m_SceneArr[i]->Init(m_pPlayer);
		}
	}
}

bool CGameManager::Render()
{
	if(m_nowScene == E_SCENE::END) return false;

	if (m_SceneArr[m_nowScene]) {
		m_SceneArr[m_nowScene]->Render();
	}
	system("cls");
	return true;
}

void CGameManager::Release()
{
	if (m_pPlayer) {
		m_pPlayer->Release();
		SAFE_DELETE(m_pPlayer);
	}

	if (m_SceneArr) {
		for (int i = 0; i < E_SCENE::END; ++i) {
			if (m_SceneArr[i]) {
				m_SceneArr[i]->Release();
				SAFE_DELETE(m_SceneArr[i]);
			}
		}
		SAFE_DELETE_ARR(m_SceneArr);
	}

	if (m_DataBase) {
		m_DataBase->Release();
		SAFE_DELETE(m_DataBase);
	}
}
