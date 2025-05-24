#pragma once
#include "CBossState.h"
class CAttackBox;
class CBaseEffect;
class CBossMeteor :
    public CBossState
{
public:
    CBossMeteor();
    ~CBossMeteor();
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;
private:
    void Meteor_Effect(CBoss* boss);
private:
    bool m_bUp;
    bool m_bEffected;
    bool m_bReady;
    bool m_bDown;
    CBaseEffect* MeteorEffect;
    CAttackBox* m_AtkBox;

};

