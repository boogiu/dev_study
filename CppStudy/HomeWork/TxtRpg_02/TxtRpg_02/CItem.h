#pragma once
#include "define.h"
class CItem
{
public:
	CItem() : m_ItemInfo{} {};
	virtual ~CItem()=0;

public:
	virtual void Init() = 0;
	virtual void Release() = 0;
protected:
	tagItem m_ItemInfo;
};

