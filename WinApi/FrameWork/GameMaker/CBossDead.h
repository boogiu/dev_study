#pragma once
#include "CBossState.h"
class CBossDead :
    public CBossState
{
public:
    CBossDead();
    ~CBossDead();

public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

private:
    float elapsedTime;
};

