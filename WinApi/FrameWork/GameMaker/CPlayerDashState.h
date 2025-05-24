#pragma once
#include "CPlayerState.h"
class CPlayerDashState :
    public CPlayerState
{
public:
    CPlayerDashState();
    ~CPlayerDashState();

    // CPlayerState을(를) 통해 상속됨
    void Enter(CPlayer* player) override;
    void Update(CPlayer* player) override;
    bool Exit(CPlayer* player) override;

private:
    float m_fDashTime;
    float m_fDashDuration;
    float m_fDashSpeed;

    int m_iDir;
    CBaseEffect* m_DashSmoke;
    ANI_FRAME Dash_Frame;
};

