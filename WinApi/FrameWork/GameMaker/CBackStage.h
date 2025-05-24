#pragma once
#include "CObject.h"
class CBackStage :
    public CObject
{
public:
    CBackStage();
    ~CBackStage();
public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();

public:
    void LoadSprite(const TCHAR* ImgKey);
private:
    bool Check(RECT tRect);
private:
    POINT prevPoint;
    float m_MovePoint;
    int bgW ;
    int bgH ;
};

