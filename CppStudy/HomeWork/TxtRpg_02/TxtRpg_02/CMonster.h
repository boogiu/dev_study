#pragma once
#include "CObject.h"

class CMonster:public CObject
{
public:
	CMonster(int _level);
	CMonster(int _level, char* Name, tagStat _Stat );

	~CMonster() override;

public:
	void Init() override;
	void Release() override;
	char* GetName() override { return m_cName; }
	int GetLevel() override { return m_iLevel; }
private:
	char* m_cName;
	int m_iLevel;
};

