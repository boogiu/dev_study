#pragma once
#include "CEquipable.h"

class CSword : public CEquipable
{
public:
	CSword(tagItem item);
	virtual ~CSword();

public:
	void Initialize()override;
	void Release()override;
	void Equip() override;

	// CEquipable을(를) 통해 상속됨
	void DeEquip() override;

};

