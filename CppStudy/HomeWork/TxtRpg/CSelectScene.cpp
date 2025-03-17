#include "CSelectScene.h"
#include "pch.h"
#include "CSceneManager.h"
#include "CPlayer.h"

CSelectScene::CSelectScene()
{
}

CSelectScene::~CSelectScene()
{
}

void CSelectScene::Initialize()
{
}

void CSelectScene::Update()
{
}

void CSelectScene::Render()
{
	cout << "플레이할 클래스를 고르십시오" << endl;

	cout << "1. 전사" << endl;
	cout << "2. 마법사" << endl;
	cout << "3. 도적" << endl;
	cout << "4. 궁수" << endl;
	int iSelect = SafeInt(1, 4);

	switch (iSelect)
	{
	case 1:
		cout << "전사를 선택하셨습니다." << endl;
		break;

	case 2:
		cout << "마법사를 선택하셨습니다." << endl;
		break;

	case 3:
		cout << "도적을 선택하셨습니다." << endl;
		break;

	case 4:
		cout << "궁수를 선택하셨습니다." << endl;
		break;

	default:
		cout << "뭐를 선택하긴 하셨군요!" << endl;
		break;
	}

	cout << "1. 이대로 진행 \t 2.다시 선택" << endl;
	int tmp =SafeInt(1, 2);

	switch (tmp)
	{
	case 1:
		CPlayer::GetInstance().SetClass(iSelect);
		CSceneManager::GetInstance().ChangerScene(SCENE::VILLAGE);
		break;

	case 2:
		cout << "다시 선택을 진행합니다." << endl;
		break;
	default:
		break;
	}
}

void CSelectScene::Release()
{
}
