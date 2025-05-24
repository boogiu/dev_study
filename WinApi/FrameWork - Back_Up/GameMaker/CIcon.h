#pragma once
#include "CBase_UI.h"
class CIcon :
    public CBase_UI
{
public:
    CIcon();
    ~CIcon();

public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();
    
public:
    void LoadSprite(const TCHAR* pImageKey);
    UI_INFO& Get_INFO() { return m_tInfo; }
private:
    int SpriteWidth;
    int SpriteHeight;
};

