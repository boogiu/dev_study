#include "CSelectScene.h"
#include "pch.h"

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
		broker.DealPC(1);
		cout << "전사를 선택하셨습니다." << endl;
		break;

	case 2:
		broker.DealPC(1);
		cout << "마법사를 선택하셨습니다." << endl;
		break;

	case 3:
		broker.DealPC(1);
		cout << "도적을 선택하셨습니다." << endl;
		break;

	case 4:
		broker.DealPC(1);
		cout << "궁수를 선택하셨습니다." << endl;
		break;

	default:
		cout << "뭐를 선택하긴 하셨군요!" << endl;
		break;
	}
	system("pause");
	nowScene = VILLAGE;
}

void CSelectScene::Release()
{
}
