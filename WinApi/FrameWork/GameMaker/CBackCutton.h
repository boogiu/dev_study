#pragma once
#include "CObject.h"
class CBackCutton  : public CObject
{
public:
    CBackCutton();
    ~CBackCutton();
public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();

public:
    void LoadSprite(const TCHAR* ImgKey);

private:
    POINT prevPoint;
    float m_MovePoint;
    int bgW;
    int bgH;
};
