#include "pch.h"
#include "CBattleScene.h"
#include "CSceneMgr.h"
#include "CGameMgr.h"

CBattleScene::CBattleScene()
{
}

CBattleScene::~CBattleScene()
{
}

void CBattleScene::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance().GetPlayer();
}

void CBattleScene::Update()
{
	//monster Pool 중 1개
	system("pause");
	CSceneMgr::GetInstance().ChangeScene(SCENE::MAZE);
}

void CBattleScene::Render()
{
	wcout << L"전투가 시작되었다!";
}

void CBattleScene::Release()
{
}
