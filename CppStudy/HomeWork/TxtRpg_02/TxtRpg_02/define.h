#pragma once

enum E_SCENE {
	START,
	VILLAGE,
	FIELD,
	SHOP,
	END
};

enum E_CLASS {
	NOCLASS,
	WARRIOR,
	MAGE,
	ROGUE,
	ARCHOR
};

#define SAFE_DELETE(p) if(p){delete p; p=nullptr;}
#define SAFE_DELETE_ARR(p) if(p){delete[] p; p = nullptr;}

struct  tagStat
{
	int m_iMaxHp;
	int m_iNowHp;
	
	int m_iAtk;
	int m_iDef;
	int m_iAvd;
	int m_iCrit;
	float m_fCritDgm;

	int m_iGold;
	int m_iExp;
};

enum E_Item {
	NONE,
	HEALTHPOTION,
	SWORD,
	ENDITEM
};
struct  tagItem
{
	E_Item category;
	int m_iLevel;
	int m_iCost;
	char* m_cName;
	char* m_cDesc;
};
#include "ctime"

#define SetRandom srand((unsigned int)time(nullptr))
#define Random(p) (rand()%p)