#include "CBattleScene.h"
#include "CPlayer.h"
#include "CFight.h"
#include "CSceneManager.h"
#include "CGameManager.h"

CBattleScene::CBattleScene()
	: monsters(nullptr)
{
}

CBattleScene::~CBattleScene()
{
}

void CBattleScene::Initialize()
{
	if (monsters != nullptr) { return; }
	monsters = new CMonster * [MONSTERS::ENDMONSTER];

	for (int i = 0; i < MONSTERS::ENDMONSTER; ++i) {
		monsters[i] = new CMonster();
		monsters[i]->Initialize(i);
	}
}

void CBattleScene::Update()
{
}

void CBattleScene::Render()
{

	while (true)
	{
		int iResult = CFight::FightMonster(&CPlayer::GetInstance(), monsters[1]);

		if (iResult == 1) { // 플레이어 승리
			cout << "당신의 승리!" << endl;
			continue;
		}		

		else if (iResult == 0) {// 플레이어 죽음
			cout << "당신의 죽음!" << endl;
			cout << "1. 마을로 돌아가기"<<endl;
			cout << "2. 종료하기"<<endl;
			int iSelect = SafeInt(1, 2);

			if (iSelect == 1) {
				CPlayer::GetInstance().Restore();
				CSceneManager::GetInstance().ChangerScene(SCENE::VILLAGE);
			}
			else {
				CGameManager::GetInstance().SetGameOn(false);
			}
			return;
		}	
	}
}

void CBattleScene::Release()
{
	for (int i = 0; i < MONSTERS::ENDMONSTER; ++i) {
		SAFE_DELETE(monsters[i]);
	}
	delete[] monsters;
}
