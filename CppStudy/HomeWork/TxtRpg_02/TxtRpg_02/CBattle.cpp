#include "pch.h"
#include "CBattle.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CGameManager.h"

bool CBattle::StartFight(CPlayer* pPlayer, CObject* right)
{
	std::cout << ">>>>" << pPlayer->GetName() << "과 " << right->GetName() << "는(은) 전투를 시작했다!" <<std:: endl;
	std::cout << std::endl;

	while (true)
	{
		pPlayer->Render();
		right->Render();
		std::cout <<"===============================================" << std::endl;

		std::cout << "1. 공격\t 2.도망" << std::endl;
		int iSelect = CFunction::SafeInt(1, 2);
		if (2 == iSelect) return true; // 전투 종료

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
			CGameManager::SceneChange(E_SCENE::VILLAGE);
			return true;
		}
		system("pause");
		system("cls");
	}
	return false;
}
