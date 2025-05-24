#include "pch.h"
#include "CObject.h"

CObject::CObject()
{
}

CObject::~CObject()
{
}

void CObject::Update_Component(float dt)
{
	for (auto& comp : components) {
		comp->Update(dt);
	}
}
