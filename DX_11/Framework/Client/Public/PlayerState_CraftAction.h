#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_CraftAction :
    public CPlayerState
{
    enum state{Start, Finished, Get, GetKeep,Putin,End};
private:
    CPlayerState_CraftAction();
    virtual ~CPlayerState_CraftAction() DEFAULT;
public:
    virtual HRESULT OnEnter() override;
    virtual void OnUpdate(_float dt) override;
    virtual HRESULT OnExit() override;
    virtual CState* HandleTransition() override;

public:
    virtual void Render_State() override;
    virtual _uint Get_InputMask() const override;
    EventMsgDesc Make_Sequence(CGameObject* pObject);

public:
    void EndDisplay();
private:
    _float m_ElapsedTime = {};
    state m_eState = { Start };
public:
    static CPlayerState_CraftAction* Create();
    virtual void Free() override;
};

NS_END
