#pragma once
#include "CObject.h"
class CBaseEffect;
class CAttackLine;

class CDimensionPierce :
    public CObject
{
public:
    CDimensionPierce();
    ~CDimensionPierce();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;


public:
    void OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType) override;
    void Set_Angle(float Angle) { m_fAngle = Angle; }
    void Set_Fire() { m_bFire = true; }
private:
    bool PierceEnd;
    bool m_bFire;
    float m_fAngle;
    float elapsedTime;
    POINT m_tTagetPos;
    CBaseEffect* m_Core;
    ANI_FRAME m_tCoreFrame;
    ANI_FRAME m_tLaserFrame;
    POINT nowPoint;
    Image* PierceImg;

    CAttackLine* m_AtkLine;
};

