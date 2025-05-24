
#pragma once
#include "CObject.h"
class BackBuilding : public CObject
{
public:
    BackBuilding();
    ~BackBuilding();
public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();

public:
    void Set_Index(int index) { m_index = index; };
public:
    void LoadSprite(const TCHAR* ImgKey);

private:
    int m_index;
};
