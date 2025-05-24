#pragma once
#include "CBaseEffect.h"

class CMonster;

class CMonsterState
{
public:
    CMonsterState();
    virtual ~CMonsterState();
public:
    virtual void Enter(CMonster* monster) PURE;
    virtual void Update(CMonster* monster) PURE;
    virtual bool Exit(CMonster* monster) PURE;
public:
    ANI_FRAME Get_AnimInfo() { return aniFrame; }
protected:
    ANI_FRAME aniFrame;
};
