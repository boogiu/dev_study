#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Get :
    public CPlayerState
{
    enum Phase{Priority, Idle, Keep,PutIn, End};
private:
    CPlayerState_Get();
    virtual ~CPlayerState_Get() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();
    virtual _uint Get_InputMask() const override;

public:
    virtual void Render_State();

private:
    EventMsgDesc Make_Sequence(CGameObject* pObject);
    void Add_Inventory(CGameObject* pObject);

private:
    Phase m_ePhase = { Priority };
    _bool m_bMsgComplete = { false };
    _bool m_bFish = { false };
public:
    static CPlayerState_Get* Create();
    virtual void Free();
};
NS_END