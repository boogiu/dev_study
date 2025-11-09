#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Talking :
    public CPlayerState
{
private:
    CPlayerState_Talking();
    virtual ~CPlayerState_Talking() DEFAULT;

public:
    virtual HRESULT OnEnter() override;
    virtual void OnUpdate(_float dt) override;
    virtual HRESULT OnExit() override;
    virtual CState* HandleTransition() override;

public:
    virtual void Render_State() override;
    virtual _uint Get_InputMask() const override;

public:
    static CPlayerState_Talking* Create();
    virtual void Free();
};
NS_END
