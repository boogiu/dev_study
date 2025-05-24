#pragma once
#include "CBossState.h"
class CEnhancedBossDead :
    public CBossState
{
public:
    CEnhancedBossDead();
   ~ CEnhancedBossDead();
public:

    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;
    void DeadEffect(CBoss* boss);
private:
    int SoundID;
};

