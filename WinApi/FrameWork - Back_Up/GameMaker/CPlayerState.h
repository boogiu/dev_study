#pragma once
class CPlayer;
class CPlayerState
{
public:
    // CState을(를) 통해 상속됨
   virtual void Enter(CPlayer* player) PURE;
   virtual void Update(CPlayer* player) PURE;
   virtual bool Exit(CPlayer* player) PURE;
};

