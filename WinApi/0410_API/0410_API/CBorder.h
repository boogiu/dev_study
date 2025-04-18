#pragma once
#include"CTile.h"
class CBorder :public CTile
{
public:
	CBorder();
	virtual ~CBorder() override;
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

