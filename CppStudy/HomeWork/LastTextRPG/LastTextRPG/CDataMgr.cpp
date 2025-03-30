#include "pch.h"
#include "CDataMgr.h"
#include <fstream>
#include <sstream>

#include "CObject.h"
#include "CMonster.h"

CDataMgr::CDataMgr()
{
}

CDataMgr::~CDataMgr()
{
}

void CDataMgr::Initialize()
{
	m_monsterData.resize(3); // 레벨 -1 만큼 리사이즈
	LoadMonster();
}

void CDataMgr::Release()
{
}

void CDataMgr::LoadPlayer()
{
	std::wfstream fs;
	fs.open("Data/playerData.csv", std::ios::in);
	wstring tmpLine;
	getline(fs, tmpLine);

	fs.close();
}

void CDataMgr::LoadMonster()
{
	std::wfstream fs;
	fs.open("Data/monsterData.csv", std::ios::in);
	wstring tmpLine;
	getline(fs, tmpLine);

	while (getline(fs, tmpLine)) {
		std::wstringstream ss(tmpLine);
		wstring token;
		vector<wstring> tmpBuffer;

		//모든 데이터 파싱하여 tmpBuffer에 저장
		while (getline(ss, token, L',')) {
			tmpBuffer.push_back(token);
		}

		int index = stoi(tmpBuffer[1]) - 1; //레벨

		m_monsterData[index].push_back(new CMonster(ApplyStat(tmpBuffer), tmpBuffer[0])); //레벨 -1에 벡터 넣어두기
	}
	fs.close();
}

void CDataMgr::LoadItem()
{
	std::wfstream fs;
	fs.open("Data/ItemData.csv", std::ios::in);
	wstring tmpLine;
	getline(fs, tmpLine);

	while (getline(fs, tmpLine)) {
		std::wstringstream ss(tmpLine);
		wstring token;
		vector<wstring> tmpBuffer;

		while (getline(ss, token, L',')) {
			tmpBuffer.push_back(token);
		}

		int index = stoi(tmpBuffer[1]) - 1;  //CATEGORY

	}
	fs.close();
}

vector<CObject*>& CDataMgr::GetMonster(int level)
{
	return m_monsterData[level-1];
}





tagStatus CDataMgr::ApplyStat(const vector<wstring>& _dataLine)
{
	tagStatus stat;

	stat.iMaxHp = stoi(_dataLine[2]);		//체력
	stat.iNormAtk = stoi(_dataLine[3]);		//일반 공격   
	stat.iMgAtk = stoi(_dataLine[4]);		//마법 공격
	stat.iNormDef = stoi(_dataLine[5]);		//일반 방버
	stat.iMgDef = stoi(_dataLine[6]);		//마법 방어
	stat.iCrit = stoi(_dataLine[7]);		//크리티컬 확률
	stat.iCritDmg = stoi(_dataLine[8]);		//크리티컬 데미지
	stat.iAvd = stoi(_dataLine[9]);		//회피율
	stat.iExp= stoi(_dataLine[10]);	//보유 경험치
	stat.iGold = stoi(_dataLine[11]);	//보유 골드

	return stat;
}
