#include "pch.h"
#include "CGameMgr.h"
#include "CDataMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"

CGameMgr::CGameMgr()
	:m_dataMgr(nullptr), m_player(nullptr)
{
}

CGameMgr::~CGameMgr()
{

}

void CGameMgr::Initialize()
{
	if (!m_dataMgr) {
		m_dataMgr = new CDataMgr;
		m_dataMgr->Initialize();
	}

	if (!m_player) {
		//플레이서 생성
		m_player = new CPlayer;
		m_player->Initialize();
	}

	CSceneMgr::GetInstance().Initialize();

}

void CGameMgr::Release()
{
	if (m_dataMgr) {
		m_dataMgr->Release();
		SAFE_DELETE(m_dataMgr);
	}

	CSceneMgr::GetInstance().Release();

	if (m_player) {
		//플레이어 삭제
		m_player->Release();
		SAFE_DELETE(m_player);
	}
}

void CGameMgr::Update()
{
	CSceneMgr::GetInstance().Update();
	if (CSceneMgr::GetInstance().GetNowScene() == SCENE::END) {
		setGameEnd(true);
	}
	system("cls");
}

void CGameMgr::Render()
{
	CSceneMgr::GetInstance().Render();
}
