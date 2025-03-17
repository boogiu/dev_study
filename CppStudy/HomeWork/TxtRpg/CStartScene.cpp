#include "CStartScene.h"
#include "pch.h"
#include "CSceneManager.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

void CStartScene::Initialize()
{
}

void CStartScene::Update()
{
}

void CStartScene::Render()
{
	cout << "텍스트 RPG _ 클래스를 곁들인" << endl;
	cout << "1. 시작하기!" << endl;
	cout << "2. 그냥 종료하기" << endl;
	int iSelect = SafeInt(1, 2);
	switch (iSelect)
	{
	case 1:
		CSceneManager::GetInstance().ChangerScene(SCENE::SELECT);
		break;
	case 2:		
		break;
	default:
		break;
	}
}

void CStartScene::Release()
{
}
