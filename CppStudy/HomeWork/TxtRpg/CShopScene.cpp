#include "CShopScene.h"
#include "pch.h"
#include "CSceneManager.h"
#include "CPlayer.h"

CShopScene::CShopScene()
{
}

CShopScene::~CShopScene()
{
}

void CShopScene::Initialize()
{
}

void CShopScene::Update()
{
}

void CShopScene::Render()
{
	cout << "당신은 상점에 들어섰다." << endl;
	cout << "상점 주인은 당신을 보고 반갑게 웃고 있다." << endl;
	cout << "소지 금액 : " << CPlayer::GetInstance().GetGold() << endl;
	cout << "====================================" << endl;
	cout << "무엇을 하시겠습니까?" << endl;
	cout << "1. 상품 구매" << endl;
	cout << "2. 강도질" << endl;
	cout << "3. 상점 나가기" << endl;
	int iSelect = SafeInt(1, 3);
	switch (iSelect)
	{
	case 1:
		cout << "=======  물건 목록 =========" << endl;
		break;
	case 2: 
		CSceneManager::GetInstance().ChangerScene(SCENE::BATTLE);
		break;
	case 3:
		CSceneManager::GetInstance().ChangerScene(SCENE::VILLAGE);
		break;
	default:
		break;
	};
}

void CShopScene::Release()
{
}

