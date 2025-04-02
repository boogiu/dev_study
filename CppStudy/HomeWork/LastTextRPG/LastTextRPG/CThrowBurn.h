#pragma once
#include "CDispoable.h"
class CThrowBurn : public CDispoable
{
public:
	CThrowBurn(tagItem item);
	~CThrowBurn() override;

public:
	void Initialize() override;
	void Release() override;
	void Active() override;
	void DeActive()override;
};

