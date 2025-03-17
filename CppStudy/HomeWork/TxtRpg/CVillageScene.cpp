#include "CVillageScene.h"
#include "pch.h"

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
	char* tmp = broker.DealPN();
	
	cout << "================================" << endl;
	if (tmp ==nullptr) {
		cout << broker.DealPN() << "이라.... 이곳에는 처음인가보군. 반갑네" << endl;
	}
	else {
		cout << broker.DealPN() <<"로군! 또 보는구만 그래!" << endl;
	}
	cout << broker.DealPN() << "그래서 무엇을 하고자 하는가?" << endl;
	cout << "================================" << endl;

	cout << "1. 구매할 물건이 있다." << endl;
	cout << "2. 잠시 숙박을 하고자 한다." << endl;
	cout << "3. 근처 사냥터를 들리고자 한다." << endl;
	cout << "4. 관심 꺼라 늙은이, 퉤" << endl;

	int iSelect = SafeInt(1, 4);
	switch (iSelect)
	{
	case 1:
		cout << "상점" << endl;

		break;

	case 2:
		cout << "여관" << endl;
		break;

	case 3:
		cout << "사냥터" << endl;
		break;

	case 4:
		cout << "전투 or 종료" << endl;
		break;

	default:
		break;
	}
	system("pause");
}

void CVillageScene::Release()
{
}
