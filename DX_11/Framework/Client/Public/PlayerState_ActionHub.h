#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_ActionHub :
    public CPlayerState
{
private:
    CPlayerState_ActionHub();
    virtual ~CPlayerState_ActionHub() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

private:
   CState* Check_ItemType();

private:
    _uint Forward_Flag = {};
public:
    static CPlayerState_ActionHub* Create();
    virtual void Free();
};

NS_END