#pragma once
#include "CBossState.h"
class CEnhancedBossSlash :
    public CBossState
{
public:
    CEnhancedBossSlash();
    ~CEnhancedBossSlash();
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;
};

