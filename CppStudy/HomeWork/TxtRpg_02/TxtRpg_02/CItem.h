#pragma once
#include "define.h"
class CItem
{
public:
	CItem() : m_ItemInfo{} {};
	virtual ~CItem();

public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Render() = 0;
public:

	virtual int GetLevel() = 0;
	virtual int GetCostl() = 0;
	virtual E_Item GetCategory() = 0;

protected:
	tagItem m_ItemInfo;
};

