#include "pch.h"
#include "CMainScene.h"
#include "CGameManager.h"
#include "CPlayer.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

void CMainScene::Init()
{
	m_pPlayer = CGameManager::GetInstance().GetPlayer();
}

void CMainScene::Render()
{

	std::cout << "네~ 텍스트 알피지 입니다~" << std::endl;
	std::cout << std::endl;
	std::cout << "===================" << std::endl;
	std::cout << "1. 시작하기 \t 2. 종료하기" << std::endl;
	std::cout << "===================" << std::endl;


	int iSelect = CFunction::SafeInt(1, 2);
	if (2 == iSelect) {
		CGameManager::GetInstance().SceneChange(E_SCENE::END);
	}
	iSelect = 0;


	system("cls");
	std::cout << ">> 직업 선택" << std::endl;
	std::cout << std::endl;
	std::cout << "===================" << std::endl;
	std::cout << "1. 전사\t: 체력과 방어력이 높은 직업" << std::endl;
	std::cout << "2. 마법사\t: 공격력이 가장 높은 직업" << std::endl;
	std::cout << "3. 도적\t: 회피율과 치명타율이 높은 직업" << std::endl;
	std::cout << "4. 궁수\t: 치명타율과 치명타 데미지가 높은 직업" << std::endl;
	std::cout << "===================" << std::endl;
	std::cout <<std::endl;
	std::cout << ">> 플레이할 직업 선택" << std::endl;


	iSelect = CFunction::SafeInt(1, 4);
	std::cout << CFunction::ReturnClass((E_CLASS)iSelect) << " 선택" <<std:: endl;

	m_pPlayer->SetClass((E_CLASS)iSelect);
	system("pause");

	CGameManager::GetInstance().SceneChange(E_SCENE::VILLAGE);
}

void CMainScene::Release()
{
}
