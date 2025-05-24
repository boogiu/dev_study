#pragma once
#include "CBossState.h"
class CEnhancedBossHoming :
    public CBossState
{
public:
    CEnhancedBossHoming();
    ~CEnhancedBossHoming();
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

};

