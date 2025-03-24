#pragma once
#include "define.h"
class CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	virtual void Init();
	void Render();
	virtual void Release();
	virtual char* GetName()=0;
public:
	int GetAtk();
	int GetGold() { return m_Stat.m_iGold; };
	int GetExp() { return m_Stat.m_iExp; };
	virtual int GetLevel()=0;
	void GetDamage(int _dmg);
	void Heal(int _heal);
	void Restore() { m_Stat.m_iNowHp = m_Stat.m_iMaxHp; }
	tagStat GetStatus();

protected:
	tagStat m_Stat;
};