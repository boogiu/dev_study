#pragma once
#include "CDispoable.h"
class CThrow: public CDispoable
{
public:
	CThrow(tagItem item);
	~CThrow() override;

public:
	void Initialize() override;
	void Release() override;
	void Active(CObject* obj) override;
	void DeActive(CObject* obj)override;

};

