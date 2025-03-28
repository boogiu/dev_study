#pragma once
#include "pch.h"
class CObject
{
public: 
	CObject();
	virtual ~CObject();

public : 
	virtual void Initialize();
	virtual void Render();
	virtual void Update();
	virtual void Release();

public:
	virtual void GetDamage();
	const wstring& GetName();

protected:
	wstring m_Sname;
	tagStatus m_stat;
};

