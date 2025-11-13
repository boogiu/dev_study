#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Diy :
    public CPlayerState
{
    CPlayerState_Diy();
    virtual ~CPlayerState_Diy() DEFAULT;

public:
    virtual HRESULT OnEnter()override;
    virtual void OnUpdate(_float dt)override;
    virtual HRESULT OnExit()override;
    virtual CState* HandleTransition() override;
    virtual _uint Get_InputMask() const override;

public:
    virtual void Render_State()override;

public:
    static CPlayerState_Diy* Create();
    virtual void Free()override;
};

NS_END