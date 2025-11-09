#pragma once
#include "Npc_HubState.h"
NS_BEGIN(Client)
class CNpcStateHub_Iteract :
    public CNpc_HubState
{
private:
    CNpcStateHub_Iteract();
    virtual ~CNpcStateHub_Iteract() DEFAULT;

public:
    HRESULT Initialize();
public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

    void DecideSubState(_float dt) override;

public:
    void Render_State() override;

public:
    static CNpcStateHub_Iteract* Create();
    virtual void Free();
};

NS_END