#include "CFieldScene.h"
#include "pch.h"
#include "CGameMgr.h"
#include "CPlayer.h"
#include "CMonster.h"

CFieldScene::CFieldScene()
	:m_MonsterArr(nullptr), m_iMonsterCount(0)
{
}

CFieldScene::~CFieldScene()
{
}

void CFieldScene::Init()
{
	m_iMonsterCount = 4;
	if (m_MonsterArr) return;

	m_MonsterArr = new CMonster * [m_iMonsterCount];

	for (int i = 0; i < m_iMonsterCount; ++i) {
		m_MonsterArr[i] = new CMonster();
		m_MonsterArr[i]->Init(i);
	}
}

void CFieldScene::Render()
{
	cout << "===================" << endl;
	cout << "당신은 사냥터에 들어섰다." << endl;
	cout << "1. 앞으로 계속 나아가자." << endl;
	cout << "2. 무서우니 돌아가자." << endl;

	int iSelect = SafeInt(1, 2);
	if (iSelect == 2) {
		ChangeScene(SCENE_ENUM::MAIN);
		return;
	}

	CPlayer*  pPlayer = CGameMgr::GetInstance()->GetPlayer();

	while (true) {
		RandSeed;
		int index = rand() % m_iMonsterCount;
		m_MonsterArr[index]->Restore();
		int iResult = FightMonster(pPlayer, m_MonsterArr[index]);
		//system("cls");
		if (iResult == 0) {
			//플레이어 패배	
			cout << "마을로 돌아갑니다" << endl;
			pPlayer->Restore();
			system("pause");
			ChangeScene(SCENE_ENUM::MAIN);
			return;
		}
		else  {
			//플레이어 승리
			cout << "당신의 승리!" << endl;
			system("pause");
			continue;
		}
	}
}

void CFieldScene::Release()
{
	if (!m_MonsterArr) return;

	for (int i = 0; i < m_iMonsterCount; ++i) {
		if (!m_MonsterArr[i]) continue;
		m_MonsterArr[i]->Release();
		SAFE_DELETE(m_MonsterArr[i]);
	}
	SAFE_DELETE_ARR(m_MonsterArr);
}

int CFieldScene::FightMonster(CPlayer* player, CMonster* monster)
{
	system("cls");
	cout << "=============================" << endl;
	cout << "야생의 몬스터와 당신이 마주쳤다!!!" << endl;
	cout << "=============================" << endl;

	cout << "1. 전투\t 2.도망" << endl;
	int iSelect = SafeInt(1, 2);

	if (iSelect == 2) {
		system("cls");
		cout << "하하 겁쟁이 같은 녀석!" << endl;
		system("pause");
		ChangeScene(SCENE_ENUM::MAIN);
		return 0 ;
	}

	while (true) {

		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
		monster->ShowStat();
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		player->GetDamage(monster->GetAtk());
		monster->GetDamage(player->GetAtk());
		cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		player->ShowStat();
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
		system("pause");

		if (!player->isAlive()) return 0;
		if (!monster->isAlive()) return 1;


	}
}

