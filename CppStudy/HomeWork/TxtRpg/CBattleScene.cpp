#include "CBattleScene.h"
#include "CPlayer.h"
#include "CFight.h"
#include "CSceneManager.h"

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
			system("pause");
			//죽음 화면으로.
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
