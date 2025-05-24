#pragma once
#include "CProjectile.h"
class CArrow :
    public CProjectile
{
public:
    CArrow();
    ~CArrow();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Set_Active(bool _active);
    void OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)override;
};

