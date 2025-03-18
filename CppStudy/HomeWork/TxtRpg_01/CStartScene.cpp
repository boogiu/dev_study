#include "CStartScene.h"
#include "CGameMgr.h"
#include "CPlayer.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

void CStartScene::Init()
{
}

void CStartScene::Render()
{
	if (2==RenderMain()) {
		CGameMgr::GetInstance()->SetGameOn(false);
		return; 
	}
	SelectClass();
	ChangeScene(SCENE_ENUM::MAIN);
}

void CStartScene::Release()
{
}

int  CStartScene::RenderMain()
{
	system("cls");
	cout << "정부기의 텍스트 알피지! 환영환영 ^__^" << endl;
	cout << "1. 시작\t 2.종료" << endl;
	return SafeInt(1, 2);
}

void CStartScene::SelectClass()
{
	system("cls");
	cout << "========직업 선택 =======" << endl;
	cout << "1.  전사 : 공격력과 체력의 밸런스" << endl;
	cout << "2. 마법사 : 공격력과 체력의 밸런스" << endl;
	cout << "3. 도적 : 공격력과 체력의 밸런스" << endl;
	cout << "4. 궁수 : 공격력과 체력의 밸런스" << endl;
	cout << "=======================" << endl;
	cout << endl;
	int iTmp = SafeInt(1, 4);
	CGameMgr::GetInstance()->GetPlayer()->SetClass((CLASS_ENUM)iTmp);
}
