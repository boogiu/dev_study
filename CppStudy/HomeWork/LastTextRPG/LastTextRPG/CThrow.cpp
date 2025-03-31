#include "pch.h"
#include "CThrow.h"
#include "CObject.h"

CThrow::CThrow(tagItem item)
{
	m_item = item;
}

CThrow::~CThrow()
{
}

void CThrow::Initialize()
{
}

void CThrow::Release()
{
}

void CThrow::Active(CObject* obj)
{
	obj->GetDebuff(PoisonAtk);
}

void CThrow::DeActive(CObject* obj)
{
}
