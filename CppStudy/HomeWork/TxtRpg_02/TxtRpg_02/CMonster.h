#pragma once
#include "CObject.h"

class CMonster:public CObject
{
public:
	CMonster(int _level);
	~CMonster() override;

public:
	void Init() override;
	void Release() override;
	char* GetName() override { return m_cName; }

private:
	char m_cName[32];
	int m_iLevel;
};

