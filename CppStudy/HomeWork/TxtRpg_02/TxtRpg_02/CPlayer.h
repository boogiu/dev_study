#pragma once
#include "CObject.h"

class CPlayer : public CObject
{
public:
	CPlayer();
	~CPlayer() override;

public:
	void Init() override;
	void Render() override;
	void Release() override;

public:
	void SetClass(E_CLASS _class);
	void ShowStatus();
	void GetExp(CObject* object);
	void LevelUp();
	void Purchase();
	void GetBooty(CObject* object);
	void SetName();
	char* GetName() { return m_cName; }
private:
	char* m_cName;
	E_CLASS m_Class;
};

