#pragma once
#include "NpcState.h"
NS_BEGIN(Client)
class CNpcState_Interact_SmallTalk :
    public CNpcState
{
private:
    CNpcState_Interact_SmallTalk();
    virtual ~CNpcState_Interact_SmallTalk() DEFAULT;

public:
    virtual HRESULT OnEnter() override;
    virtual void OnUpdate(_float dt) override;
    virtual HRESULT OnExit() override;
    virtual CState* HandleTransition() override;


public:
    static CNpcState_Interact_SmallTalk* Create();
    virtual void Free() override;
};

NS_END
