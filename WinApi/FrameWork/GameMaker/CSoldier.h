#pragma once
#include "CMonster.h"
class CMonsterState;

class CSoldier :
    public CMonster
{
public:
    CSoldier();
    ~CSoldier();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType) override;

public:
    void Find_Target(CObject* target) override;
    void Not_Find_Target() override;

private:
    float m_AttakDelay;
    float m_ElapsedTime;
};

