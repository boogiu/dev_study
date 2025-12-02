#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_TransferGive :
    public CPlayerState
{
    enum state{IDLE, PullOut, Passing ,PutIn, End};
private:
    CPlayerState_TransferGive();
    virtual ~CPlayerState_TransferGive() DEFAULT;

public:
    virtual HRESULT OnEnter() override;
    virtual void OnUpdate(_float dt) override;
    virtual HRESULT OnExit() override;
    virtual CState* HandleTransition() override;

public:
    virtual void Render_State() override;
    virtual _uint Get_InputMask() const override;

private:
    state m_eState = { IDLE };
    _float m_fOutime = { IDLE };
    TRANS_ITEM m_ItemData = {};
public:
    static CPlayerState_TransferGive* Create();
    virtual void Free();
};

NS_END
