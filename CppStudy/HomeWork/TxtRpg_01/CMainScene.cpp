#include "CMainScene.h"
#include "pch.h"

#include "CGameMgr.h"
#include "CPlayer.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

void CMainScene::Init()
{
}

void CMainScene::Render()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->GetPlayer();
	cout << "마을의 촌장이 당신에게 다가옵니다." << endl;

	if (pPlayer->GetName()) {
		cout << "아, 또 보는군. " << pPlayer->GetName() << endl;
	}
	else {
		AskName();
		pPlayer->SetName();
		cout  << pPlayer->GetName()<<"라고 하는구만. 기억하겠네." << endl;
		cout <<"그래서," << pPlayer->GetName() << "군은 ";

	}
	AskDirection();
}

void CMainScene::Release()
{
}

void CMainScene::AskName() {
	cout << "미안하지만 자네는 처음 보는 듯 하군." << endl;
	cout << "이름이 무엇인지 물어보아도 되겠는가?" << endl;
}

void CMainScene::AskDirection()
{
	cout << "무엇을 하려던 참인가?" << endl;
	cout << "==============선택지===============" << endl;
	cout << "1. 사냥터를 가려고 한다." << endl;
	cout << "2. 말걸지 말아라 노인네" << endl;

		switch (SafeInt(1, 2))
		{
		case 1:
			ChangeScene(SCENE_ENUM::FIELD);
			break;

		case 2:
			ChangeScene(SCENE_ENUM::END);
			break;

		default:
			break;
		}
}
