#pragma once
#include "NpcState.h"
NS_BEGIN(Client)

class CNpcState_Interact_TransItem :
    public CNpcState
{
    enum State{PullOut, ItemHandle,Transferred, End};
private:
    CNpcState_Interact_TransItem();
    virtual ~CNpcState_Interact_TransItem() DEFAULT;

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
    TRANS_ITEM m_ItemData = {};

public:
    static CNpcState_Interact_TransItem* Create();
    virtual void Free() override;
};

NS_END