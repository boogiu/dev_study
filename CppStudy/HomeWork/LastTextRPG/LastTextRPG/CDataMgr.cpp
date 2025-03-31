#include "pch.h"
#include "CDataMgr.h"
#include <fstream>
#include <sstream>

#include "CObject.h"
#include "CMonster.h"
#include "CPlayer.h"

#include "Items.h"

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
	LoadItem();
}

void CDataMgr::Release()
{
}

void CDataMgr::LoadPlayer(CLASS _class, CPlayer* player)
{
	std::wfstream fs;
	fs.open("Data/playerData.csv", std::ios::in);
	wstring tmpLine;

	for (int i = 0; i < static_cast<int>(_class)+1; i++) {
		//enum만큼 줄 뛰어넘기
		getline(fs, tmpLine);
	}

	wstringstream ss(tmpLine);
	wstring token;
	vector<wstring> tmpBuffer;

	while (getline(ss, token, L',')) {
		tmpBuffer.push_back(token);
	}

	player->SetClass(ApplyStat(tmpBuffer), _class);
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

		if (tmpBuffer[3] == L"HEALTHPOTION") {
			m_DisposItem.push_back(new CHealthPotion(ApplyInfo(tmpBuffer)));
		}
		else if (tmpBuffer[3] == L"THROWVENOM") {
			m_DisposItem.push_back(new CThrow(ApplyInfo(tmpBuffer)));
		}

	}
	fs.close();
}

vector<CObject*>& CDataMgr::GetMonster(int level)
{
	return m_monsterData[level - 1];
}

vector<CItem*>& CDataMgr::GetDisposData()
{
	return m_DisposItem;
}

vector<CItem*>& CDataMgr::GetEquipData()
{
	return m_EquipItem;
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
	stat.iExp = stoi(_dataLine[10]);	//보유 경험치
	stat.iGold = stoi(_dataLine[11]);	//보유 골드

	return stat;
}

tagItem CDataMgr::ApplyInfo(const vector<wstring>& _dataLine)
{
	// NAME	LEVEL	COST	 CATEGORY	POINT	Disposable 	DESCRIPT

	tagItem tmpInfo;

	tmpInfo.m_Name = _dataLine[0];
	tmpInfo.m_Level = _dataLine[1];
	tmpInfo.m_Cost = stoi(_dataLine[2]);
	tmpInfo.m_Point = stoi(_dataLine[4]);
	tmpInfo.m_Desc = _dataLine[6];

	return tmpInfo;
}
