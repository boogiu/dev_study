#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_AimWalk :
    public CPlayerState
{
private:
    CPlayerState_AimWalk();
    virtual ~CPlayerState_AimWalk() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();
public:
    void Render_State() override;
public:
    virtual _uint Get_InputMask() const override;

public:
    static CPlayerState_AimWalk* Create();
    virtual void Free();
};

NS_END