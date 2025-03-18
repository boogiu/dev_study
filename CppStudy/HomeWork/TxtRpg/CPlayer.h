#ifndef _PLAYER_
#define _PLAYER_
#include "pch.h"

class CPlayer
{

	SINGLETON(CPlayer)

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

	void SetName();
	char* GetName();

	void SetClass(int _class);
	int GetGold();
	int GetAtk() { return m_iAtk; }
	int GetHp() { return m_iNowHp; }

	void GetDamage(int Dmg);
	void Restore();

private :
	char* m_cName;
	int m_iGold;
	
	int m_iMaxHp;
	int m_iNowHp;

	int m_iAtk;
};

#endif // !_PLAYER_
