#include "CGameManager.h"
#include "pch.h"

CGameManager::CGameManager()
	:m_isGameOn(true)
{
}

CGameManager::~CGameManager()
{
}

void CGameManager::Initialize()
{
	CSceneManager::GetInstance();
	CPlayer::GetInstance();

	CSceneManager::GetInstance().Initialize(); 
	CPlayer::GetInstance().Initialize();
}

void CGameManager::Update()
{
	CSceneManager::GetInstance().Update();
}

void CGameManager::Render()
{
	CSceneManager::GetInstance().Render();
}

void CGameManager::Release()
{
	CSceneManager::GetInstance().Release();
	CPlayer::GetInstance().Release();
}
