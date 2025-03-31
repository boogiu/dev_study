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
	CGameMgr::GetInstance().SetClass(static_cast<CLASS>(iSelect));
	CSceneMgr::GetInstance().ChangeScene(SCENE::VILLAGE);
}

void CClassScene::Render()
{
	wcout << L"1. 전사 : 체력과 방어력의 베이스가 좋은 직업" << endl;
	wcout << L"2. 마법사 : 마법 공격력과 화상 효과를 부여하는 직업" << endl;
	wcout << L"3. 도적 : 높은 크리티컬 확률과 독 데미지를 부여하는 직업" << endl;
	wcout << L"4. 궁수 : 높은 크리티컬 확률과 높은 데미지를 가진 직업" << endl;
}

void CClassScene::Release()
{
}
