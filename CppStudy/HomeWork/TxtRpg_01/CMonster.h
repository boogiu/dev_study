#ifndef __MONSTER_H__
#define __MONSTER_H__
#include "define.h"

class CMonster
{
public : 
	CMonster();
	~CMonster();

public :
	void Init(int index);
	void Render();
	void Release();

	void ShowStat();
	void GetDamage(int dmg);
	int GetAtk() { return m_mInfo.m_iAtk; }

	void Restore();
	bool isAlive() { return !(m_mInfo.m_iNowHp <= 0); }

private:
	char m_cName[32];
	tagInfo m_mInfo;
};

#endif // !__MONSTER_H__
