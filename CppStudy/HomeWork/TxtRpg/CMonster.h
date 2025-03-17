#ifndef _MONSTER_
#define _MONSTER_

class CMonster
{

public:
	CMonster();
	~CMonster();
public:
	void Initialize(int index);
	char* GetName() { return m_cName; }
	int GetAtk() { return m_iAtk; }
	int GetHp() { return m_iHp; }

	void GetDamage(int _dmg);
private:
	char m_cName[32];
	int m_iHp;
	int m_iAtk;
};

#endif // !_MONSTER_

