#pragma once
#include "CGameObject.h"
class CPlatform : public CGameObject
{
public:
	CPlatform();
	~CPlatform();

public:
	void Initialize();
	void Update();
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;
};

