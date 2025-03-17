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
	default:
		break;
	}

}

void CSelectScene::Release()
{
}
