#include "pch.h"
#include "CBattle.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CGameManager.h"

//true시 싸움 종료
bool CBattle::StartFight(CPlayer* pPlayer, CObject* right)
{
	system("cls");
	std::cout << ">>>>" << pPlayer->GetName() << "과 " << right->GetName() << "는(은) 전투를 시작했다!" <<std:: endl;
	std::cout << std::endl;

	while (true)
	{
		std::cout << "===============================================" << std::endl;
		right->Render();
		std::cout << "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_" << std::endl;
		pPlayer->Render();
		std::cout <<"===============================================" << std::endl;

		std::cout << "1. 공격\t 2.도망\t 3.내 스탯보기" << std::endl;
		int iSelect = CFunction::SafeInt(1, 3);
		if (2 == iSelect) {
			return true; // 전투 종료
		}
		else if (3 == iSelect) {
			system("cls");
			pPlayer->ShowStatus();
			system("pause");
			continue;
		}

		std::cout << ">>>>" << pPlayer->GetName() << "의 공격! " << std::endl;
		right->GetDamage(pPlayer->GetAtk());

		if (right->GetStatus().m_iNowHp <= 0) {
			std::cout << ">>>>" << right->GetName() << "는 사망했다! " << std::endl;
			std::cout << ">>>>" << pPlayer->GetName() << "의 승리! " << std::endl;
			system("pause");

			pPlayer->GetBooty(right);

			system("cls");
			return false;
		}

		std::cout << ">>>>" << right->GetName() << "의 공격! " << std::endl;
		pPlayer->GetDamage(right->GetAtk());

		if (right->GetStatus().m_iNowHp <= 0) {
			std::cout << ">>>>" << pPlayer->GetName() << "는 사망했다! " << std::endl;
			system("pause");
			system("cls");
			std::cout << ">>>> 마을로 돌아갑니다. " << std::endl;
			pPlayer->Heal(pPlayer->GetStatus().m_iMaxHp);
			CGameManager::GetInstance().SceneChange(E_SCENE::END);
			return true;
		}
		system("pause");
		system("cls");
	}
	return false;
}
