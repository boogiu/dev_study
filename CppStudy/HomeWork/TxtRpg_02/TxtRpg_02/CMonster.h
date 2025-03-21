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
	int m_iLevel;
	char m_cName[32];
};

