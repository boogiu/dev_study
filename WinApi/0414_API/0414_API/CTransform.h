#pragma once
#include "CComponent.h"
class CTransform :
	public CComponent
{
public:
	CTransform();
	~CTransform();

public:
	// CComponent을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public:
	VECTOR2& GetPos() { return m_vPos; }
	void SetPos(VECTOR2 _pos) { m_vPos = _pos; }
	VECTOR2& GetScale() { return m_vScale; }
	void SetScale(VECTOR2 _scale) { m_vScale = _scale; }

private:
	void Update_Rect(HDC _hDC);
private:
	VECTOR2 m_vPos;
	VECTOR2 m_vScale;
};

