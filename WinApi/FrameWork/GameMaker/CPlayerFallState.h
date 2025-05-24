#pragma once
#include "CPlayerState.h"
class CPlayerFallState :
    public CPlayerState
{
public:
    CPlayerFallState();
    ~CPlayerFallState();

public:
    // CPlayerState을(를) 통해 상속됨
    void Enter(CPlayer* player) override;
    void Update(CPlayer* player) override;
    bool Exit(CPlayer* player) override;

private:
    bool m_bGround;
};

