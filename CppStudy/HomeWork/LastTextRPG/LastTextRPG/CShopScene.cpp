#include "pch.h"
#include "CShopScene.h"
#include "CSceneMgr.h"
#include "CGameMgr.h"
#include "CItem.h"
#include "CPlayer.h"

CShopScene::CShopScene()
{
}

CShopScene::~CShopScene()
{
}

void CShopScene::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance().GetPlayer();
}

void CShopScene::Update()
{
	int iSelect = CInput::SafeInt(1, 3);
	if (iSelect == 3)
	{
		CSceneMgr::GetInstance().ChangeScene(SCENE::VILLAGE);
	}
	else {
		ShowItem(iSelect);
	}
}

void CShopScene::Render()
{
	wcout << L"반갑네. 어떤 상점을 이용하고 싶은가?"<<endl;
	wcout << L"1. 일회용품 상점 \t 2. 장비 상점 \t 3. 돌아가기";

}

void CShopScene::Release()
{
}

void CShopScene::ShowItem(int i)
{
	vector<CItem*> tmp;

	if (i == 1) {
		tmp = CGameMgr::GetInstance().GetDisposItem();
	}
	else if (i == 2) {
		tmp = CGameMgr::GetInstance().GetEquipItem();
	}

	while (true)
	{
		wcout << L"소지금 : " << m_pPlayer->GetGold() << L"G" << endl;

		if (tmp.empty()) {
			wcout << L"미안 하군.. 아직 상점이 준비가 되지 않은 모양이네" << endl;
			system("pause");
			return;
		}
		wcout << L"=====================================" << endl;
		for (int i = 0; i < tmp.size(); ++i) {
			wcout << i+1 << L". ";
			tmp[i]->Render();		
			wcout << L"=====================================" << endl;
		}
		wcout << L"이 중에 어떤 것을 구매하고 싶은가?" << endl;
		wcout << L"번호를 선택. (0번 선택시 뒤로 돌아가기)" << endl;

		int iSelect = CInput::SafeInt(0, tmp.size());
		if (iSelect == 0) { return; }
		else {
			m_pPlayer->Purchase(tmp[iSelect - 1]);
		}
		system("cls");
	}
}
