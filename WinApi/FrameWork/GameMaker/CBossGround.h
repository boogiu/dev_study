#pragma once
#include "CBossState.h"
class CBossGround :
    public CBossState
{
public:
    CBossGround();
   ~ CBossGround();

public:

    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

    void Smoke(CBoss* boss);
    void LinearEffect(CBoss* boss);
private:
    bool m_bSmoke;
    bool m_bReady;
    bool isMoving;
    bool m_bMoved;
    float m_ReadyTime;
    float m_MoveEnd;
    int m_Dir;
    CAttackBox* m_AtkBox;
};

