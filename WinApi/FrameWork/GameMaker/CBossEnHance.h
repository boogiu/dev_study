#pragma once
#include "CBossState.h"
class CBossEnHance :
    public CBossState
{
public:
    CBossEnHance();
    ~CBossEnHance();

public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

    private:
        void SetFlame(CBoss* boss);
        void SetElect(CBoss* boss);
private:
    bool m_bGround;
    bool m_bWaken;
    int soundID;

};

