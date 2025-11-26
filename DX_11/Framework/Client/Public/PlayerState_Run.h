#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Run :
    public CPlayerState
{
private:
    CPlayerState_Run();
    virtual ~CPlayerState_Run() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();
public:
    void Render_State() override;
public:
    virtual _uint Get_InputMask() const override;

private:
    void Request_Dust();
private:
    _bool m_bFliping = { false };
    _float2 m_vLastAxis = {};
    _float m_fDuration = {};
    _float m_fRunTime = {};
public:
    static CPlayerState_Run* Create();
    virtual void Free();
};

NS_END