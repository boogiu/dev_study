#include "pch.h"
#include "CGameManager.h"

#include "Scenes.h"
#include "Items.h"

#include "CPlayer.h"
#include "CDataBase.h"

CGameManager::CGameManager()
	:  m_pPlayer(nullptr), m_SceneArr(nullptr),m_DataBase(nullptr),m_nowScene(E_SCENE::START)
{
}

CGameManager::~CGameManager()
{
}

void CGameManager::Init()
{
	//플레이어 생성
	if (!m_pPlayer) {
		m_pPlayer = new CPlayer();
		m_pPlayer->Init();
	}

	//씬 생성
	if (!m_SceneArr) {
		m_SceneArr = new CScene * [E_SCENE::END];
		//각 인덱스별로 씬 객체 할당해주기.
		m_SceneArr[E_SCENE::START] = new CMainScene;
		m_SceneArr[E_SCENE::VILLAGE] = new CVillage;
		m_SceneArr[E_SCENE::FIELD] = new CField;
		m_SceneArr[E_SCENE::SHOP] = new CShopScene;

	}

	//씬 초기화
	for (int i = 0; i < E_SCENE::END; ++i) {
		if (m_SceneArr[i]) {
			m_SceneArr[i]->Init();
		}
	}

	//데이터베이스 불러오기
	if (!m_DataBase) {
		m_DataBase = new CDataBase;
		m_DataBase->Init();
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
	//데이터 베이스 반환
	if (m_DataBase) {
		m_DataBase->Release();
		SAFE_DELETE(m_DataBase);
	}

	//씬 반환
	if (m_SceneArr) {
		for (int i = 0; i < E_SCENE::END; ++i) {
			if (m_SceneArr[i]) {
				m_SceneArr[i]->Release();
				SAFE_DELETE(m_SceneArr[i]);
			}
		}
		SAFE_DELETE_ARR(m_SceneArr);
	}

	//플레이어 반환
	if (m_pPlayer) {
		m_pPlayer->Release();
		SAFE_DELETE(m_pPlayer);
	}
}

CPlayer* CGameManager::GetPlayer()
{
	if (m_pPlayer) {
		return m_pPlayer;
	}
	return nullptr;
}

CObject* CGameManager::GetMonster(int _level)
{
	if (!m_DataBase) {
		return nullptr;
	}
	return m_DataBase->GetRandMonster(_level);
}

 const std::vector<CItem*>& CGameManager::GetItems()const
{
	return m_DataBase->GetItem();
}
