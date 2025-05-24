#pragma once
#include "CObject.h"
class CDmgText :
    public CObject
{
public:
    CDmgText();
    ~CDmgText();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Set_Text(float dmg);

private:
    bool m_bAppear;
    float m_OriginY;
    float m_elapsedTime;
    wstring m_szBuffer;
};

