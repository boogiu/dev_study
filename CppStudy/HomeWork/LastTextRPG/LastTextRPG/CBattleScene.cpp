#include <random>
#include "pch.h"
#include "CBattleScene.h"
#include "CSceneMgr.h"
#include "CGameMgr.h"
#include "CObject.h"
#include "CPlayer.h"

CBattleScene::CBattleScene()
{
}

CBattleScene::~CBattleScene()
{
}

void CBattleScene::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance().GetPlayer();
}

void CBattleScene::Update()
{
	vector<CObject*> vecMonster = CGameMgr::GetInstance().GetMonsterPool(1);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, vecMonster.size() - 1);

	//monster Pool 중 1개
	if (Fight(m_pPlayer, vecMonster[dis(gen)])) {

		CSceneMgr::GetInstance().ChangeScene(SCENE::MAZE);
	}
}

void CBattleScene::Render()
{

}

void CBattleScene::Release()
{
}

bool CBattleScene::Fight(CPlayer* player, CObject* compete)
{
	wcout << L"전투가 시작되었다." << endl;
	compete->Initialize();
	player->SetCompete(compete);

	while (true)
	{
		compete->Render();
		player->Render();

		wcout << endl;
		wcout << L"1. 공격\t2. 가방 \t3. 스킬 \n\n4. 나의 정보 \t5. 도망" << endl;

		int iSelect = CInput::SafeInt(1, 5);
		system("cls");

		if (iSelect == 5) return true;
		
		else if (iSelect == 4) {
			player->ShowStatus();
			continue;
		}

		else if (iSelect == 3) {
			player->ShowSkill();
		}
		else if (iSelect == 2) {
			player->ShowInventory();
		}

		else if (iSelect ==1) {
			player->GetDamage(compete);
			compete->GetDamage(player);
		}

		if (player->CheckDead()) {
			wcout << L"플레이어의 사망..." << endl;
			player->SetCompete(nullptr);

			system("pause");
			CSceneMgr::GetInstance().ChangeScene(SCENE::END);
			return false;
		}

		if (compete->CheckDead()) {
			wcout << L"플레이어의 승리!" << endl;
			player->SetCompete(nullptr);

			system("pause");
			player->GetProps(compete);
			return true;
		}

	}
}
