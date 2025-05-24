#pragma once
#include "CBossState.h"
class CHomingPierce;

class CBossHoming :
    public CBossState
{
public:
    CBossHoming();
    ~CBossHoming();
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

private:
    float anim_out;
    float m_fTagetAngle;
    int m_Homing;
    int m_HomingCount;
    bool m_Targeting;
    bool AllFired;
    float m_elapsedTime;
    CHomingPierce* m_pierce[3];
};

