#pragma once
#include "CObj.h"
class CBullet :
    public CObj
{
public:
    CBullet( short sDir);
    virtual ~CBullet() override;

public:
    // CObj을(를) 통해 상속됨
    void Release() override;
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;

public:

private:
    void MoveBullet();
private:
    short m_Dir; 
};

