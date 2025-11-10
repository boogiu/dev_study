#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Interact :
    public CPlayerState
{
private:
    CPlayerState_Interact();
    virtual ~CPlayerState_Interact() DEFAULT;

public:
    virtual HRESULT OnEnter()override;
    virtual void OnUpdate(_float dt)override;
    virtual HRESULT OnExit()override;
    virtual CState* HandleTransition()override;

public:
    virtual void Render_State()override;
    virtual _uint Get_InputMask() const override;

public:
    virtual void OnCollisionEnter(COLLISION_CONTEXT context) override;
    virtual void OnCollisionStay(COLLISION_CONTEXT context) override;

private:
    _bool m_bTalkComplete = { false };
public:
    static CPlayerState_Interact* Create();
    virtual void Free()override;
};

NS_END
