#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)

class CPlayerState_PoleAction :
    public CPlayerState
{
    enum state {BEGIN,AIR,THROW,AIM, CHECK, HIT, PULLBACK, CATCH, GET ,END};
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
    void Get_Event(const BaseEvent& event) ;
private:
    state m_eState = { BEGIN };
    class CEventSystem* m_pEvtSys = { nullptr };
    _float m_HitTime = {};
    _float4 m_LeftHand = {};

 public:
    static CPlayerState_PoleAction* Create();
    virtual void Free();
};

NS_END