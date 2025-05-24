#pragma once
#include "CBossState.h"
class CBossIdle :
    public CBossState
{
public:
    CBossIdle();
    ~CBossIdle();
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

private:
    bool m_bWaitAnim;
};

//600 x 400 y