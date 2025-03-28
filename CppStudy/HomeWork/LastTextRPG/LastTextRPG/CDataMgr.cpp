#include "pch.h"
#include "CDataMgr.h"
#include <fstream>

CDataMgr::CDataMgr()
{
}

CDataMgr::~CDataMgr()
{
}

void CDataMgr::Initialize()
{
	m_itemData.reserve(10);
	m_monsterData.reserve(10);
}

void CDataMgr::Release()
{
}

void CDataMgr::LoadPlayer()
{
}

void CDataMgr::LoadMonster()
{
	std::wfstream fs;
	fs.open("Data/monsterData.csv", std::ios::in);
	wstring tmpLine;
	getline(fs,tmpLine);

}

void CDataMgr::LoadItem()
{
	std::wfstream fs;
	fs.open("Data/ItemData.csv", std::ios::in);
}
