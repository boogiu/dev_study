#pragma once
#include "CGameObject.h"
class CBullet : public CGameObject
{
public:
	CBullet(VECTOR2 _pos);
	~CBullet();


public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void CalcOrbit();

private:
	float m_fRadian;
	float m_fRadius;

	VECTOR2 m_Center;
};

