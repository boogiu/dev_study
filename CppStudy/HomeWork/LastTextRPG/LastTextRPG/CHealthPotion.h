#pragma once
#include "CDispoable.h"
class CHealthPotion : public CDispoable
{
public:
	CHealthPotion(tagItem item);
	~CHealthPotion() override;

public:
	void Initialize() override;
	void Release() override;
	void Active(CObject* obj) override;
	void DeActive(CObject* obj)override;

};

