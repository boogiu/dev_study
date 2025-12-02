#pragma once
#include "NpcState.h"
NS_BEGIN(Client)

class CNpcState_Interact_ReceiveItem :
    public CNpcState
{
    enum State{Receive, Get,Putin, End};
private:
    CNpcState_Interact_ReceiveItem();
    virtual ~CNpcState_Interact_ReceiveItem() DEFAULT;

public:
    virtual HRESULT OnEnter() override;
    virtual void OnUpdate(_float dt) override;
    virtual HRESULT OnExit() override;
    virtual CState* HandleTransition() override;

public:
    void Render_State() override;
    void OnClose(_bool isEnd);

private:
    State m_eState = { End };
    _float4x4 m_SocketMatrix = {};
    RESPONSE_TRANS_ITEM m_ItemData = {};

public:
    static CNpcState_Interact_ReceiveItem* Create();
    virtual void Free() override;
};

NS_END