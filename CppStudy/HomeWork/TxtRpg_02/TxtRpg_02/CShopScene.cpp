#include "pch.h"
#include "CShopScene.h"
#include "CGameManager.h"
#include "CPlayer.h"
#include "CItem.h"

CShopScene::CShopScene()
{
}

CShopScene::~CShopScene()
{
}

void CShopScene::Init()
{
	m_pPlayer = CGameManager::GetInstance().GetPlayer();
}

void CShopScene::Render()
{
	std::cout << ">>>>당신은 상점에 들어섰습니다." << std::endl;
	std::cout << ">>>>상점 주인은 당신을 심드렁하게 바라보며 말을 꺼냅니다." << std::endl;
	std::cout << std::endl;

	std::cout << "\" 상점 주인 \"" << std::endl;
	std::cout << "거, 필요한 거라도 있으신가?" << std::endl;
	std::cout << std::endl;
	std::cout << "1. 일단 목록이라도 봅시다." << std::endl;
	std::cout << "2. 필요한 것이 없으니 다시 나가자." << std::endl;
	std::cout << "당신의 소지금 : " << m_pPlayer->GetGold() << std::endl;

	int iSelect = CFunction::SafeInt(1, 2);
	if (iSelect == 2) {
		CGameManager::GetInstance().SceneChange(E_SCENE::VILLAGE);
		return;
	}
	while (true) {
		system("cls");
		std::cout << "당신의 소지금 : " << m_pPlayer->GetGold() << std::endl;
		iSelect = 0;
		iSelect = SelectLevel();
		if (iSelect == 4) {
			CGameManager::GetInstance().SceneChange(E_SCENE::VILLAGE);
			return;
		}
		else {
			ShowItems(iSelect);
		}
	}
}

void CShopScene::Release()
{
}

int CShopScene::SelectLevel()
{
	std::cout << "\"" << m_pPlayer->GetName() << " \"" << std::endl;
	std::cout << "목록이라도 보여주쇼." << std::endl;

	std::cout << "\" 상점 주인 \"" << std::endl;
	std::cout << "끙...귀찮군. 어느정도 급의 아이템을 원하나?" << std::endl;
	std::cout << std::endl;

	std::cout << "1. 저렴한 것들 위주로 목록을 보여주쇼" << std::endl;
	std::cout << "2. 적당히 괜찮은 상품들을 보고 싶소만." << std::endl;
	std::cout << "3. 나는 최고의 제품만 취급한다." << std::endl;
	std::cout << "4. 갑자기 흥미가 떨어졌소." << std::endl;

	int iSelect = CFunction::SafeInt(1, 4);
	return iSelect;

}

void CShopScene::ShowItems(int _level) {
	system("cls");
	std::cout << "당신의 소지금 : " << m_pPlayer->GetGold() << std::endl;
	std::vector<CItem*> tmp;

	for (CItem* ptr : CGameManager::GetInstance().GetItems()) {
		if (ptr->GetLevel() == _level) {
			tmp.push_back(ptr);
		}
	}

	if (tmp.empty()) {
		std::cout << "이런...재고가 없군 그래. 나중에 다시 오게나." << std::endl;
		system("pause");
		return;
	}

	for (int i = 0; i < tmp.size(); ++i) {
		if (tmp[i]->GetLevel() == _level) {
			std::cout << "-----------------------------------------------" << std::endl;
			std::cout << i+1 << ". ";
			tmp[i]->Render();
			std::cout << "-----------------------------------------------" << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "이 중에 구매하고 싶은 것이 있는가?\n 번호로 말해주시게나" << std::endl;
	std::cout << ">>"<<tmp.size()+1<<". 뒤로 돌아가기" << std::endl;

	int iSelect = CFunction::SafeInt(1, tmp.size() + 1);
	if (iSelect == tmp.size()+1) {
		return;
	}
	
	bool purchase =m_pPlayer->Purchase(tmp[iSelect-1]);

	if (!purchase) {
		std::cout << "소지금이 부족합니다." << std::endl;
	}
	else {
		std::cout << "성공적으로 구매했습니다!." << std::endl;
	}
	system("pause");
}