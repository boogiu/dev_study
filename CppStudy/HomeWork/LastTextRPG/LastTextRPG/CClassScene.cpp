#include "pch.h"
#include "CClassScene.h"
#include "CSceneMgr.h"
#include "CGameMgr.h"
#include "CPlayer.h"

CClassScene::CClassScene()
{
}

CClassScene::~CClassScene()
{
}

void CClassScene::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance().GetPlayer();
}

void CClassScene::Update()
{
	int iSelect = CInput::SafeInt(1, 4);
	m_pPlayer->SetClass(static_cast<CLASS>(iSelect));

	CSceneMgr::GetInstance().ChangeScene(SCENE::VILLAGE);
}

void CClassScene::Render()
{
	wcout << L"1. 전사 : " << endl;
	wcout << L"2. 마법사 : " << endl;
	wcout << L"3. 도적 : " << endl;
	wcout << L"4. 궁수 : " << endl;
}

void CClassScene::Release()
{
}
