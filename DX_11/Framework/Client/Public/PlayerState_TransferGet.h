#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_TransferGet :
    public CPlayerState
{
    enum state{IDLE, Transfered, Passing ,PutIn, End};
private:
    CPlayerState_TransferGet();
    virtual ~CPlayerState_TransferGet() DEFAULT;

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
    RESPONSE_TRANS_ITEM m_data = {};
public:
    static CPlayerState_TransferGet* Create();
    virtual void Free();
};

NS_END
