#pragma once
#include "CBossState.h"
class CBossSlash :
    public CBossState
{
public:
    CBossSlash();
   virtual  ~CBossSlash() override;
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

private:
    bool m_slashed;
    float m_elapsedTime;
};

