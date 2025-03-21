#pragma once
#include "CObject.h"

class CPlayer : public CObject
{
public:
	CPlayer();
	~CPlayer() override;

public:
	void Init() override;
	void Release() override;

public:
	void SetClass(E_CLASS _class);
	void ShowStatus();
	void LevelUp();
	void Purchase();
	void GetBooty(CObject* object);
	void SetName();
	char* GetName() override { return m_cName; }
private:
	char* m_cName;
	E_CLASS m_Class;
	int m_iLevel;
	int m_iMaxExp;
};

