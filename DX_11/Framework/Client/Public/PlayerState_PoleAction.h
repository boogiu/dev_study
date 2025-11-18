#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)

class CPlayerState_PoleAction :
    public CPlayerState
{
private:
    CPlayerState_PoleAction();
    virtual ~CPlayerState_PoleAction() DEFAULT;

public:
    virtual HRESULT OnEnter() override;
    virtual void OnUpdate(_float dt) override;
    virtual HRESULT OnExit() override;
    virtual CState* HandleTransition() override;

public:
    virtual void Render_State() override;
    virtual _uint Get_InputMask() const override;
private:
    _bool isCathced = {};
public:
    static CPlayerState_PoleAction* Create();
    virtual void Free();
};

NS_END