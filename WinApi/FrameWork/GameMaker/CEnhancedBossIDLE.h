#pragma once
#include "CBossState.h"
class CEnhancedBossIDLE :
    public CBossState
{
public:
    CEnhancedBossIDLE();
    ~CEnhancedBossIDLE();

public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

};

