#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_TransItem :
    public CPlayerState
{
private:
    CPlayerState_TransItem();
    virtual ~CPlayerState_TransItem() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

private:
    _bool m_bTransed = { false };
public:
    static CPlayerState_TransItem* Create();
    virtual void Free();
};
NS_END
