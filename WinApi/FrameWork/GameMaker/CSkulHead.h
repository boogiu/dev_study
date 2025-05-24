#pragma once
#include "CProjectile.h"
class CSkulHead : public CObject
{
public:
    CSkulHead();
    ~CSkulHead();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Set_Active(bool _active);
    void OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)override;
    void OnCollisionEnter(CObject* object, RECT collRECT) override;
    void OnCollisionWall(int dir) override;

private:
    Gdiplus::Image* skullImg;
    bool isHitted;
    bool OnSound;
    float m_fAngle;
};
