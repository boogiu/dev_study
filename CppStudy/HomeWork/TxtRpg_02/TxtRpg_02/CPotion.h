#pragma once
#include "CItem.h"
class CPlayer;

class CPotion : public CItem
{
public:
	CPotion();
	virtual ~CPotion();
	virtual void Activate(CPlayer* _player) =0;
};

