#include "pch.h"
#include "CSceneMgr.h"
#include "CMainScene.h"
#include "CGameMgr.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

void CMainScene::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance().GetPlayer();
}

void CMainScene::Update()
{
	switch (CInput::SafeInt(1, 2))
	{
	case 1:
		CSceneMgr::GetInstance().ChangeScene(SCENE::CLASS);
		break;
	case 2:
		CSceneMgr::GetInstance().ChangeScene(SCENE::END);
		break;

	default:
		break;
	}
}

void CMainScene::Render()
{
	wcout << L"텍스트 RPG" << endl;
	wcout << L"1. 시작하기 \t 2. 종료하기" << endl;
}

void CMainScene::Release()
{
}
