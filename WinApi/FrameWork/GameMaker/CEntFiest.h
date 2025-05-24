#pragma once
#include "CProjectile.h"
class CAnim;
class CEntFiest :
    public CProjectile
{
public:
    CEntFiest();
    ~CEntFiest();
public:
    // CProjectile을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType);
public:
    CAnim* Get_Anim() { return m_pAnim; }
    void Fire() { m_bFire = true; };

private:
    bool m_bFire;
    bool m_bSound;
    int m_soundID;
    float elapseTime;
    CAnim* m_pAnim;
    ANI_FRAME fiestFrame;
};


