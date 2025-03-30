#pragma once
#include "CObject.h"
class CMonster:public CObject
{
public:
	CMonster(const tagStatus& stat, const std::wstring& name);
	~CMonster() override;
public:
	void Initialize() override;
	void Update()override;
	void Render() override;
	void Release() override;
};

