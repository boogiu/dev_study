#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_NetSwing :
    public CPlayerState
{
private:
    CPlayerState_NetSwing();
    virtual ~CPlayerState_NetSwing() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();
    virtual _uint Get_InputMask() const override;

public:
    virtual void OnCollisionEnter(COLLISION_CONTEXT context) override;
    virtual void OnCollisionStay(COLLISION_CONTEXT context) override;

private:
    _bool isCathced = {};
    _bool isSoundComplete = {};
public:
    static CPlayerState_NetSwing* Create();
    virtual void Free();
};
NS_END
