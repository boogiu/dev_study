#pragma once
#include "PlayerState.h"

NS_BEGIN(Client)
class CPlayerState_Eat :
    public CPlayerState
{
private:
    CPlayerState_Eat();
    virtual ~CPlayerState_Eat() DEFAULT;

public:
    virtual HRESULT OnEnter()override;
    virtual void OnUpdate(_float dt)override;
    virtual HRESULT OnExit()override;
    virtual CState* HandleTransition()override;
    virtual _uint Get_InputMask() const override;

public:
    virtual void Render_State()override;
private:
    _bool m_SoundComplete = { false };
public:
    static CPlayerState_Eat* Create();
    virtual void Free()override;
};
NS_END