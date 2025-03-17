#include "CVillageScene.h"
#include "pch.h"
#include "CSceneManager.h"
#include "CPlayer.h"

CVillageScene::CVillageScene()
{
}

CVillageScene::~CVillageScene()
{
}

void CVillageScene::Initialize()
{
}

void CVillageScene::Update()
{
}

void CVillageScene::Render()
{
	cout << "당신은 마을에 당도하였습니다." << endl;
	system("cls");

	if (!CPlayer::GetInstance().GetName()) {
		cout << "늙수구레하게 생긴 노인이 당신에게 다가와 이름을 묻습니다." << endl;
		cout << "================================" << endl;
		cout << "김촌장 : " << endl;
		cout << "처음 보는 청년이구만, 이름이 뭔가?" << endl;
		cout << "================================" << endl;
		CPlayer::GetInstance().SetName();
		cout << "당신은 스스로를 \""<< CPlayer::GetInstance().GetName()<<"\"라고 소개했다." << endl;

	}
	cout << "김촌장 : " << endl;
	cout << "아," << CPlayer::GetInstance().GetName() << "이로군. 그래!  무슨 일인가?" << endl;
	cout << "================================" << endl;

	cout << "1. 구매할 물건이 있다." << endl;
	cout << "2. 잠시 숙박을 하고자 한다." << endl;
	cout << "3. 근처 사냥터를 들리고자 한다." << endl;
	cout << "4. 관심 꺼라 늙은이, 퉤" << endl;

	int iSelect = SafeInt(1, 4);
	switch (iSelect)
	{
	case 1:
		cout << "그렇군. 허허, 상점은 저기 왼쪽 골목에 있으니 참고하게." << endl;
		CSceneManager::GetInstance().ChangerScene(SCENE::SHOP);
		break;

	case 2:
		cout << "그렇군. 허허, 여관은 저기 오른쪽 골목에 있으니 참고하게." << endl;
		CSceneManager::GetInstance().ChangerScene(SCENE::MOTEL);
		break;

	case 3:
		cout << "그렇군. 허허, 앞으로 쭈욱 나아가게. 몸 조심하고 말이야." << endl;
		CSceneManager::GetInstance().ChangerScene(SCENE::BATTLE);
		break;

	case 4:		
		cout << "이런 싸가지 없는 놈의 애새끼!" << endl;
		CSceneManager::GetInstance().ChangerScene(SCENE::BATTLE);
		break;
	default:
		break;
	}
	system("pause");
}

void CVillageScene::Release()
{
}
