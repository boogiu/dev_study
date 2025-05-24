#pragma once
#include "CObject.h"
class CParticle :
    public CObject
{
public:
    CParticle();
    ~CParticle();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

    SIZE GetBitmapSizeFromDC(HDC hMemDC);

public:
    void Set_Name(wstring name, int i);

private:
    Gdiplus::Image* Parts;
    wstring m_szName;
    SIZE m_tSize;
    float m_fAngleSpeed; // CParticle ¸â¹ö¿¡ Ãß°¡
    float m_fAngle;
};

