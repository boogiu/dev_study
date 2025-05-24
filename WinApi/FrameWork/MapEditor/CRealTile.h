#pragma once
#include "CTile.h"

class CRealTile : public CTile
{
public:
	CRealTile();
	~CRealTile();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;
};

