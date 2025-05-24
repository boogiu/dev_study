#pragma once
#include "CPlayerState.h"
class CPlayerMoveState :
    public CPlayerState
{
public:
    CPlayerMoveState();
    ~CPlayerMoveState();

public:
    // CPlayerState을(를) 통해 상속됨
    void Enter(CPlayer* player) override;
    void Update(CPlayer* player) override;
    bool Exit(CPlayer* player) override;
};

