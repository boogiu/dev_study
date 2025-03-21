#include "pch.h"
#include "CField.h"'
#include "CGameManager.h"
#include "CBattle.h"
#include "CObject.h"
#include "CMonster.h"

CField::CField()
	:m_MonsterArr(nullptr), m_iSpawnCnt(3)
{
}

CField::~CField()
{
}

void CField::Init(CPlayer* _pPlayer)
{
	m_pPlayer = _pPlayer;
}

void CField::Render()
{
	std::cout << ">> 당신은 한참을 걷던 도중, 사냥터 입구에 도달했습니다." << std::endl;
	std::cout << ">> 눈 앞에는 세 갈림길이 보입니다." << std::endl;

	std::cout <<std::endl;
	std::cout << " 1. 왼쪽 길 (비교적 안전해보인다.)" << std::endl;
	std::cout << " 2. 중앙 길 (때때로 어려움이 있어보인다.)" << std::endl;
	std::cout << " 3. 오른쪽 길 (불안함 감각이 느껴진다.)" << std::endl;
	std::cout << " 4. 우선 마을로 다시 돌아가자." << std::endl;
	std::cout << std::endl;

	int iSelect = CFunction::SafeInt(1, 4);

	switch (iSelect)
	{
	case 1:
		std::cout << ">> 당신은 초보 사냥터로 향했다." << std::endl;
		system("pause");
		MonsterPool(iSelect);
		break;
	case 2:
		std::cout << ">> 당신은 중급 사냥터로 향했다." << std::endl;
		system("pause");
		MonsterPool(iSelect);
		break;
	case 3:
		std::cout << ">> 당신은 고급 사냥터로 향했다." << std::endl;
		system("pause");
		MonsterPool(iSelect);
		break;
	case 4:
		std::cout << ">> 당신은 마을로 돌아갔습니다." << std::endl;
		system("pause");
		CGameManager::SceneChange(E_SCENE::VILLAGE);
		return;
	default:
		break;
	}

	system("cls");
	bool BattleEnd = false;

	while (!BattleEnd)
	{
		SetRandom;
		int iSpawn = Random(m_iSpawnCnt);
		BattleEnd = CBattle::StartFight(m_pPlayer, m_MonsterArr[iSpawn]);
		m_MonsterArr[iSpawn]->Restore();
	}
}

void CField::Release()
{
	if (!m_MonsterArr) return;
	for (int i = 0; i < m_iSpawnCnt;i++) {
		if (m_MonsterArr[i]) {
			m_MonsterArr[i]->Release();
			SAFE_DELETE(m_MonsterArr[i]);
		}
	}
	SAFE_DELETE_ARR(m_MonsterArr);
}

void CField::MonsterPool(int _level)
{
	Release(); //우선 다 해제

	m_MonsterArr = new CObject *[m_iSpawnCnt];

	for (int i = 0; i < m_iSpawnCnt; i++) {
		m_MonsterArr[i] = new CMonster(_level);
		m_MonsterArr[i]->Init();
	}
}
