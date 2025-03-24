#include "pch.h"
#include "CVillage.h"
#include "CPlayer.h"
#include "CGameManager.h"
CVillage::CVillage()
{
}

CVillage::~CVillage()
{
}

void CVillage::Init()
{
	m_pPlayer = CGameManager::GetInstance().GetPlayer();
}

void CVillage::Render()
{
	std::cout << ">> 당신은 눈 앞에 보이는 마을로 향했습니다." << std::endl;
	std::cout << ">> 마을의 촌장이 당신에게 다가옵니다." << std::endl;
	std::cout << std::endl;
	if (!m_pPlayer->GetName()) {
		std::cout << ">> 마을의 촌장이 당신의 이름을 물어보고 있습니다." << std::endl;
		std::cout <<std::endl;
		std::cout << "\"촌장\"" << std::endl;
		std::cout << "----" << std::endl;
		std::cout << "처음 보는 인물이군. 이름을 물어보아도 되겠는가?" << std::endl;
		m_pPlayer->SetName();
		std::cout << std::endl;
		std::cout << "\"당신\"" << std::endl;
		std::cout <<"나의 이름은 "<< "\""<< m_pPlayer->GetName()<< "\" 이다." << std::endl;
		std::cout << std::endl;
		std::cout << "\"촌장\"" << std::endl;
		std::cout << "그렇군! 앞으로 기억하겠네."  << std::endl;
	}
	else {
		std::cout << "\"촌장\"" << std::endl;
		std::cout << m_pPlayer->GetName()<<", 또 보는군." << std::endl;
	}
	std::cout <<"무슨 용무로 마을에 들렀는가?." << std::endl;
	std::cout << std::endl;

	std::cout << "1. 상점을 들리고자 한다." << std::endl;
	std::cout << "2. 사냥터를 이용하고자 한다." << std::endl;
	std::cout << "3. 게임 그만할래요..." << std::endl;

	int iSelect = CFunction::SafeInt(1, 3);
	if (3 == iSelect) {
		CGameManager::GetInstance().SceneChange(E_SCENE::END);
	}
	else if (2 == iSelect) {
		CGameManager::GetInstance().SceneChange(E_SCENE::FIELD);
	}
	else if (1 == iSelect) {
		CGameManager::GetInstance().SceneChange(E_SCENE::SHOP);
	}
}

void CVillage::Release()
{
}
