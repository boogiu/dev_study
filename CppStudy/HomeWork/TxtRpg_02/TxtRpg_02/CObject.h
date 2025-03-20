#pragma once
class CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	virtual void Init();
	virtual void Render();
	virtual void Release();

public:
	int GetAtk();
	void GetDamage(int _dmg);
	void Heal(int _heal);
	tagStat GetStatus();

protected:
	tagStat m_Stat;
};