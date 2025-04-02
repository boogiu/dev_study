#pragma once
#include "CItem.h"
class CDispoable : public CItem
{
public:
	CDispoable();
	virtual ~CDispoable();

public:
	virtual void Initialize();
	virtual void Release();
	virtual void Active() = 0;
	virtual void DeActive() = 0;
private:


};

