#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_ChopTree :
    public CPlayerState
{
private:
    CPlayerState_ChopTree();
    virtual ~CPlayerState_ChopTree() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();
public:
    virtual _uint Get_InputMask() const override;

private:
    _bool m_bOnTree = {};
    _bool m_bInCycle = {};
public:
    static CPlayerState_ChopTree* Create();
    virtual void Free();
};
NS_END
