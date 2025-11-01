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
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();
    virtual _uint Get_InputMask() const override;

private:
    void Make_Hole();
    void Burry_Hole();

private:
    _bool m_isTree = { false };
    _bool m_isStone = { false };
    _bool m_isDigged = { false };
    _bool m_isDiggable = { false };
    _bool m_DigComplete = { false };
public:
    static CPlayerState_Dig* Create();
    virtual void Free();
};
NS_END
