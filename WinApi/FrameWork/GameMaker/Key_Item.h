#pragma once
#include "CObject.h"
class Key_Item :
    public CObject
{
public:
	Key_Item();
	~Key_Item();
public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;
public:
	void Set_Name(wstring skulName);
	void Set_Origin(float yPos) { m_fOriginY = yPos; };
	SIZE GetBitmapSizeFromDC(HDC hMemDC);

private:
	float m_fFloatAmplitude;     // 위아래 이동 거리
	float m_fFloatSpeed;         // 속도 (진동 주기)
	float m_fOriginY;      // 원래 위치 기억
	float elapsedTime;
	wstring m_IConName;
	SIZE m_spSize;
};


