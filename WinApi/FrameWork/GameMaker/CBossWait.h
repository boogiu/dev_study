#pragma once
#include "CBossState.h"

class CBossWait : public CBossState
{
public:
    CBossWait();
    ~CBossWait();
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

private :
    POINT m_point;
    bool m_bWaiting;
    bool m_bWaitEnd;

    float m_elapsedTime;
};
