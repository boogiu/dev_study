#pragma once
#include "CObj.h"
class CMonster :
    public CObj
{
public:
    CMonster();
    virtual ~CMonster() override;

public:
    // CObj을(를) 통해 상속됨
    void Release() override;
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    
public:
    void MonsterMove();
    void GetHit();
    bool IsDead();
private:

private:
    bool m_bDead;
    TCHAR m_szHp[32];
    float m_fMoveRange;
    float m_fNowMoveRange;
    int m_NowDir;

    int m_Hp;

};

