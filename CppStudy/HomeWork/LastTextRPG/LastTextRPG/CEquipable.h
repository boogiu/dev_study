#pragma once
#include "CItem.h"

class CEquipable : public CItem
{
	public:
		CEquipable();
		virtual ~CEquipable();

	public:
		virtual void Initialize();
		virtual void Release();
		virtual void Equip() = 0;
		virtual void DeEquip() = 0;
		bool GetEquiped() { return isEquiped; }
		void SetEquiped(bool equiped) {  isEquiped = equiped; }

private:
	bool isEquiped;
};

