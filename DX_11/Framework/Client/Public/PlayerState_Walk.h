#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Walk :
    public CPlayerState
{
private:
    CPlayerState_Walk();
    virtual ~CPlayerState_Walk() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();
    virtual _uint Get_InputMask() const override;

private:
    void Request_Dust();
private:
    _float m_fWalkTime = {  };

public:
    static CPlayerState_Walk* Create();
    virtual void Free();
};

NS_END