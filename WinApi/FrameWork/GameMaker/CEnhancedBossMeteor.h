#pragma once
#include "CBossState.h"
class CEnhancedBossMeteor :
    public CBossState
{
public:
    CEnhancedBossMeteor();
    ~CEnhancedBossMeteor();

public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;


private:
    void Sign(CBoss* boss);
    void Impact(CBoss* boss);
    void Elect(CBoss* boss);


private:
    bool m_bUp;
    bool m_bReady;
    bool m_bDown;
    float correctionY;
};

