#include "pch.h"
#include "CShopScene.h"
#include "CSceneMgr.h"
#include "CGameMgr.h"

CShopScene::CShopScene()
{
}

CShopScene::~CShopScene()
{
}

void CShopScene::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance().GetPlayer();
}

void CShopScene::Update()
{
	CSceneMgr::GetInstance().ChangeScene(SCENE::VILLAGE);

}

void CShopScene::Render()
{
	wcout << L"상점 구현 중 ---";
	system("pause");
}

void CShopScene::Release()
{
}
