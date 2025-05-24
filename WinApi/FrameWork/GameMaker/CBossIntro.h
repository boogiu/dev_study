#pragma once
#include "CBossState.h"
class CBossIntro :
    public CBossState
{
public:
    CBossIntro();
    ~CBossIntro();
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

private:
    void ChangeIntro();
private :
    bool m_bTakeOff;
    bool m_bJumped;
    bool m_bGround;

};

