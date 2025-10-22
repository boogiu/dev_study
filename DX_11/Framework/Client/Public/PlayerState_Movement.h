#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Movement :
    public CPlayerState
{
private:
    CPlayerState_Movement();
    virtual ~CPlayerState_Movement() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    _bool isFliping() { return m_bFliping; }
    _bool CheckMovable(_float4 nextPos);
    _float Get_CurrentDegree() { return m_fTurnDegree; };
    _float Get_TargetDegree() { return m_fTargetDegree; };

    void    Set_CurrentDegree(_float degree)     {  m_fTurnDegree = degree; };
    void    Set_TargetDegree(_float degree)     { m_fTargetDegree = degree; };
public:
    virtual void Render_State();

private:
    _float m_fTurnDegree = {};
    _float m_fTargetDegree = {};
    _bool m_bFliping = { false };
public:
    static CPlayerState_Movement* Create();
    virtual void Free();
};

NS_END