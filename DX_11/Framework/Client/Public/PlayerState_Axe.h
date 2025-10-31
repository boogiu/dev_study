#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Axe :
    public CPlayerState
{
private:
    CPlayerState_Axe();
    virtual ~CPlayerState_Axe() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();
    virtual _uint Get_InputMask() const override;

private:
    vector<_uint> indexes;
public:
    static CPlayerState_Axe* Create();
    virtual void Free();
};
NS_END