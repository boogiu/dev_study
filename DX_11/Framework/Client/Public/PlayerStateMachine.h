#pragma once
#include "StateMachine.h"
#include "Player.h"

NS_BEGIN(Client)

class CPlayerStateMachine :
    public CStateMachine
{
private:
    CPlayerStateMachine(CPlayer* pPlayer);
    virtual ~CPlayerStateMachine() DEFAULT;

public:
    HRESULT Initialize();

public:
    void Update(_float dt) override;
    virtual void Request_ChangeState(STATE_LAYER eLayer, const string& NextState) override;

public:
    void Render_State(class CPlayer* pPlayer);
    _uint Get_CurrentMask(STATE_LAYER eLayer);
private:
    CPlayer* m_pOwner = { nullptr };

public:
    static CPlayerStateMachine* Create(CPlayer* pPlayer); 
    virtual void Free();
};

NS_END