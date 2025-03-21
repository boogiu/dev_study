#pragma once
#include "CItem.h"
class CPlayer;

class CPotion : public CItem
{
public:
	virtual ~CPotion() =0;
	virtual void Activate(CPlayer* _player) =0;

};

