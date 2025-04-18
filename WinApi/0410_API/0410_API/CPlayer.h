#pragma once
#include "CObject.h"


class CPlayer :
    public CObject
{
public:
    CPlayer();
    virtual ~CPlayer() override;
public:
    // CObject을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Render(HDC _hDC) override;
    virtual void Release() override;

public:
    virtual void OnCollisionEnter(CObject* _object) override;
    void Activate() override;
    void DeActivte() override;
    virtual void OnHit() override;

private:
    void Update_State();
    void CheckKey();
    void MovePlayer();
    void JumpPlayer();
    void FireBullet();

private:
    
    TCHAR m_szBuffer[64];
    float m_fJumpPow;
    int m_xDir;
    CObject* m_Radar;
    ULONGLONG m_OnHitTime;
    ULONGLONG ShotPrevTime;
    ULONGLONG ShotCool;
};

