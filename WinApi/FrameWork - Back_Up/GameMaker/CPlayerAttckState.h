#pragma once
#include "CPlayerState.h"
class CPlayerAttckState :
    public CPlayerState
{
public:
    CPlayerAttckState();
    ~CPlayerAttckState();

    // CPlayerState을(를) 통해 상속됨
    void Enter(CPlayer* player) override;
    void Update(CPlayer* player) override;
    bool Exit(CPlayer* player) override;

private:
    float m_fAtkTime;
    float m_fAtkDuration;
    float m_fAtkSpeed;

    int m_iDir;
};

