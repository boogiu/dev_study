#pragma once
#include "CPlayerState.h"
class CAttackBox;
class CPlayerJumpAtkState :
    public CPlayerState
{
public:
    CPlayerJumpAtkState() : m_pAtkBox(nullptr), sound(false){};
    ~CPlayerJumpAtkState();
    // CPlayerState을(를) 통해 상속됨
    void Enter(CPlayer* player) override;
    void Update(CPlayer* player) override;
    bool Exit(CPlayer* player) override;
private:
    CAttackBox* m_pAtkBox;
    bool sound;
};

