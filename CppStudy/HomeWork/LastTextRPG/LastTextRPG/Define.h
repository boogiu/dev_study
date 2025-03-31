#pragma once


#define SAFE_DELETE(p) if(p){delete p; p=nullptr;}
#define SAFE_DELETE_ARR(p) if(p){delete[] p; p=nullptr;}

enum class SCENE {
	MAIN,
	CLASS,
	VILLAGE,
	SHOP,
	MOTEL,
	MAZE,
	BATTLE,
	END
};

enum class CLASS {
	NONE,
	WARRIOR,
	MAGE,
	ROGUE,
	ARCHOR
};

enum class KEY
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ENTER,
	ESC
};



struct  tagStatus
{
	int iMaxHp;
	int iNormAtk;
	int iMgAtk;
	int iNormDef;
	int iMgDef;
	int iCrit;
	int iCritDmg;
	int iAvd;
	int iExp;
	int iGold;
};

struct tagItem
{
	int m_Cost;
	int m_Point;
	std::wstring m_Name;
	std::wstring m_Desc;
	std::wstring m_Level;
};


//AtkType
constexpr int NormalAtk = 1<< 0;
constexpr int MagicalAtk = 1 << 1;
constexpr int BurnAtk = 1 << 2;
constexpr int PoisonAtk = 1 << 3;

//StateType
