#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Dig :
    public CPlayerState
{
private:
    CPlayerState_Dig();
    virtual ~CPlayerState_Dig() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();
    virtual _uint Get_InputMask() const override;

private:
    void Make_Hole();

private:
    _bool m_isTree = { false };
    _bool m_isDigged = { false };
    _bool m_isDiggable = { false };
    _bool m_DigComplete = { false };
public:
    static CPlayerState_Dig* Create();
    virtual void Free();
};
NS_END
