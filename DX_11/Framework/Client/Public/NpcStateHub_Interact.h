#pragma once
#include "Npc_HubState.h"
NS_BEGIN(Client)
class CNpcStateHub_Interact :
    public CNpc_HubState
{
private:
    CNpcStateHub_Interact();
    virtual ~CNpcStateHub_Interact() DEFAULT;

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
    static CNpcStateHub_Interact* Create();
    virtual void Free();
};

NS_END