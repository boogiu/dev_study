#include "CFight.h"
#include "pch.h"

int CFight::FightMonster(CPlayer* _player, CMonster* _monster) //0 패배 , 1 승리
{
	cout << _player->GetName() << "과(와)" << _monster->GetName() << "가 전투를 시작했다!" << endl;
	while (true)
	{
		cout << _monster->GetName() << "의 공격! <<<<" << _monster->GetAtk() << "만큼의 데미지!" << endl;
		_player->GetDamage(_monster->GetAtk());
		cout << _player->GetName() << "의 공격! <<<<" << _player->GetAtk() << "만큼의 데미지!" << endl;
		_monster->GetDamage(_player->GetAtk());
		system("pause");

		if (_player->GetHp() <= 0) { 
			return 0; 
		}
		if (_monster->GetHp() <= 0) { 
			return 1; 
		}
	}
}
