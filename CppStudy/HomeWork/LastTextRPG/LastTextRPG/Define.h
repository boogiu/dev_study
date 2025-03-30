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

//AtkType
constexpr int Nmal = 0 << 0;
constexpr int Magical = 0 << 0;
constexpr int Burn = 1 << 1;
constexpr int Poison = 1 << 2;