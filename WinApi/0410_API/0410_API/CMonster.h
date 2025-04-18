#pragma once
#include "CObject.h"
class CMonster :
    public CObject
{
public:
    CMonster();
    virtual ~CMonster() override;
public:
    // CObject을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Render(HDC _hDC) override;
    virtual void Release() override;
public:
    virtual void Activate() override;
    virtual void DeActivte() override;
    virtual void OnCollisionEnter(CObject* _object) override;

public:
    void SetTarget(CObject* _target);

private:
    void ChaseTarget();
    void MoveMonster();
    void OnHit();
    void Update_State();

private : 
    bool m_bChaseMod;
    ULONGLONG m_OnHitTime;
    CObject* m_target;
   };

