#include "pch.h"
#include <fstream>
#include <sstream>

#include "CDataBase.h"
#include "CPlayer.h"
#include "CMonster.h"

using namespace std;

CDataBase::CDataBase()
	:m_ItemArr(nullptr),m_MonsterArr(nullptr)
{
}

CDataBase::~CDataBase()
{
}

void CDataBase::Init()
{
}

void CDataBase::Release()
{

}

void CDataBase::LoadPlayer(CPlayer* pPlayer)
{

}

void CDataBase::SavePlayer(CPlayer* pPlayer)
{
}

void CDataBase::SetClass(CPlayer* pPlayer)
{

}

void CDataBase::LoadMonster()
{
	fstream fs;
	fs.open("monster.csv", ios::in);
	//Çì´õ ¶Ù±â
	//NAME, CLASS, HP, DEF,	AVD, CRIT, CRITDMG, GOLD, EXP
	
	while () {
		stringstream ss;
	}

}

CObject** CDataBase::GetMonster()
{
	return nullptr;
}

void CDataBase::LoadCItem()
{
}

CItem** CDataBase::GetItem()
{
	return nullptr;
}
