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
	virtual void Active(CObject* obj) = 0;
	virtual void DeActive(CObject* obj) = 0;
private:


};

