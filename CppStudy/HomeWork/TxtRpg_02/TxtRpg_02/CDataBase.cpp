#include "pch.h"
#include "CDataBase.h"
#include "Items.h"
#include "CPlayer.h"
#include "CMonster.h"

using namespace std;

CDataBase::CDataBase()
{
}

CDataBase::~CDataBase()
{
}

void CDataBase::Init()
{
	LoadMonster();
	LoadCItem();
}

void CDataBase::Release()
{
		for (int i = 0; i < m_monsterVec.size(); ++i) {
			if (m_monsterVec[i]) {
				m_monsterVec[i]->Release();
				SAFE_DELETE(m_monsterVec[i])
			}
		}
		for (int i = 0; i < m_ItemArr.size(); ++i) {
			if (m_ItemArr[i]) {
				m_ItemArr[i]->Release();
				SAFE_DELETE(m_ItemArr[i])
			}
		}
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
	std::fstream fs;
	fs.open("Data/MonsterData.csv", std::ios::in);
	
	//헤더 뛰기
	//NAME, LEVEL, HP,ATK, DEF,	AVD, CRIT, CRITDMG, GOLD, EXP
	string line;
	getline(fs, line);  // 첫 번째 줄(헤더) 스킵

	while (getline(fs, line)) {  // 한 줄씩 읽기
		stringstream ss(line);
		string token;
		vector<string> tmpBuffer;

		while (getline(ss, token, ','))// ',' 기준으로 데이터 분리
		{  		tmpBuffer.push_back(token);		}
	
		string nameStr = tmpBuffer[0]; // 문자열을 동적 할당하여 유지
		char* cName = new char[nameStr.length() + 1];
		strcpy_s(cName, nameStr.length() + 1, nameStr.c_str());

		int iLV = stoi(tmpBuffer[1]);

		// tagStat 초기화
		tagStat tmpStat = {};
		tmpStat.m_iMaxHp = stoi(tmpBuffer[2]);  // HP
		tmpStat.m_iNowHp = tmpStat.m_iMaxHp;  // HP
		tmpStat.m_iAtk = stoi(tmpBuffer[3]);    // ATK
		tmpStat.m_iDef = stoi(tmpBuffer[4]);    // DEF
		tmpStat.m_iAvd = stoi(tmpBuffer[5]);    // AVD
		tmpStat.m_iCrit = stoi(tmpBuffer[6]);   // CRIT
		tmpStat.m_fCritDgm = stof(tmpBuffer[7]); // CRITDMG
		tmpStat.m_iGold = stoi(tmpBuffer[8]);   // GOLD
		tmpStat.m_iExp = stoi(tmpBuffer[9]);    // EXP

		// 몬스터 배열에 추가
		//m_MonsterArr[count]= new CMonster(iLV, cName, tmpStat);
		CObject* tmpPtr= new CMonster(iLV, cName, tmpStat);
		tmpPtr->Init();
		m_monsterVec.push_back(tmpPtr);
	}
}

CObject* CDataBase::GetRandMonster(int _level)
{
	vector<CObject*> filtered;
	for (int i = 0; i < m_monsterVec.size();++i) {
		if (m_monsterVec[i]->GetLevel() == _level)
			filtered.push_back(m_monsterVec[i]);
	}

	if (filtered.empty()) { return nullptr; }

	int index =Random(filtered.size());
	
	return filtered[index];
}

void CDataBase::LoadCItem()
{
	std::fstream fs;
	fs.open("Data/ItemData.csv", std::ios::in);

	string line;//줄 단위
	getline(fs, line);

	while (getline(fs, line))
	{
		stringstream ss(line);
		string token; //임시 버퍼
		vector<string> tmpBuffer; //구분버퍼

		while (getline(ss, token, ',')) {
			tmpBuffer.push_back(token);
		}

		tagItem tmpItem;
		// NAME	LEVEL	COST CATEGORY		POINT	설명(dump) 	DESCRIPT
	
		string nameStr = tmpBuffer[0]; // 문자열을 동적 할당하여 유지
		tmpItem.m_cName = new char[nameStr.length() + 1];
		strcpy_s(tmpItem.m_cName, nameStr.length() + 1, nameStr.c_str());

		tmpItem.m_iLevel = stoi(tmpBuffer[1]);
		tmpItem.m_iCost = stoi(tmpBuffer[2]);
		string category = tmpBuffer[3];
		int point = stoi(tmpBuffer[4]);

		string DescStr = tmpBuffer[6]; // 문자열을 동적 할당하여 유지
		tmpItem.m_cDesc = new char[DescStr.length() + 1];
		strcpy_s(tmpItem.m_cDesc, DescStr.length() + 1, DescStr.c_str());

		if ("HEALTHPOTION" == category) {
			tmpItem.category = E_Item::HEALTHPOTION;
			CItem* tmp = new CHealthPotion(tmpItem,point);
			tmp->Init();
			m_ItemArr.push_back(tmp);
		}

		if ("SWORD" == category) {
			tmpItem.category = E_Item::SWORD;
			CItem* tmp = new CSword(tmpItem, point);
			tmp->Init();
			m_ItemArr.push_back(tmp);
		}
	}
}

const std::vector<CItem*>& CDataBase::GetItem() const
{
	// TODO: 여기에 return 문을 삽입합니다.
	return  m_ItemArr;
}

