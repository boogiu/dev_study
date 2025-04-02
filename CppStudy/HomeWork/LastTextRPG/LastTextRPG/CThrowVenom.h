#pragma once
#include "CDispoable.h"
class CThrowVenom: public CDispoable
{
public:
	CThrowVenom(tagItem item);
	~CThrowVenom() override;

public:
	void Initialize() override;
	void Release() override;
	void Active() override;
	void DeActive()override;
};

