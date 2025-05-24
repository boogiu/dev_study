#pragma once
#include "CPlayerState.h"
class CPlayerJumpState :
    public CPlayerState
{
public :
    CPlayerJumpState();
   ~CPlayerJumpState();

public:
   void Enter(CPlayer* player) override;
   void Update(CPlayer* player) override;
   bool Exit(CPlayer* player) override;

private:
   CBaseEffect* JumpEffect;
   ANI_FRAME jumpFrame;
};

