#pragma once
#include "CObject.h"
class CProjectile :
	public CObject
{
public:
	CProjectile();
	virtual ~CProjectile();

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;
};

